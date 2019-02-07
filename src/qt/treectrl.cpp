/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/treectrl.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"
#include "wx/treectrl.h"
#include "wx/imaglist.h"

#include "wx/qt/private/winevent.h"
#include "wx/qt/private/treeitemfactory.h"

#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QHeaderView>
#include <QtGui/QPainter>

namespace
{
    struct TreeItemDataQt
    {
        TreeItemDataQt() : data(NULL)
        {
        }
        TreeItemDataQt(wxTreeItemData* data) : data(data)
        {
            if ( !registered )
            {
                qRegisterMetaTypeStreamOperators<TreeItemDataQt>("TreeItemDataQt");
                registered = true;
            }
        }
        wxTreeItemData *data;
        static bool registered;
    };
    bool TreeItemDataQt::registered = false;
    Q_DECLARE_METATYPE(TreeItemDataQt)

    QDataStream &operator<<(QDataStream &out, const TreeItemDataQt &)
    {
        return out;
    }
    QDataStream &operator>>(QDataStream &in, TreeItemDataQt &)
    {
        return in;
    }

    QTreeWidgetItem *wxQtConvertTreeItem(const wxTreeItemId &item)
    {
        return static_cast<QTreeWidgetItem*>(item.GetID());
    }

    wxTreeItemId wxQtConvertTreeItem(QTreeWidgetItem *item)
    {
        return wxTreeItemId(item);
    }

    size_t CountChildren(QTreeWidgetItem *item)
    {
        const int currentCount = item->childCount();
        size_t totalCount = currentCount;

        for (int i = 0; i < currentCount; ++i)
        {
            totalCount += CountChildren(item->child(i));
        }

        return totalCount;
    }

}


class ImageState
{
public:
    ImageState()
    {
        for (int i = wxTreeItemIcon_Normal; i < wxTreeItemIcon_Max; ++i)
        {
            m_imageStates[i] = -1;
        }   
    }

    int &operator[](size_t index)
    {
        return m_imageStates[index];
    }

    const int &operator[](size_t index) const
    {
        return m_imageStates[index];
    }

    int m_imageStates[wxTreeItemIcon_Max];
    
};

class wxQTreeWidget : public wxQtEventSignalHandler<QTreeWidget, wxTreeCtrl>
{
public:
    wxQTreeWidget(wxWindow *parent, wxTreeCtrl *handler) :
        wxQtEventSignalHandler(parent, handler),
        m_editorFactory(handler)
    {
        connect(this, &QTreeWidget::currentItemChanged, this, &wxQTreeWidget::OnCurrentItemChanged);
        connect(this, &QTreeWidget::itemActivated, this, &wxQTreeWidget::OnItemActivated);
        connect(this, &QTreeWidget::itemClicked, this, &wxQTreeWidget::OnItemClicked);
        connect(this, &QTreeWidget::itemCollapsed, this, &wxQTreeWidget::OnItemCollapsed);
        connect(this, &QTreeWidget::itemExpanded, this, &wxQTreeWidget::OnItemExpanded);
        connect(this, &QTreeWidget::itemChanged, this, &wxQTreeWidget::OnItemChanged);

        m_editorFactory.AttachTo(this);
        setDragEnabled(true);
        viewport()->setAcceptDrops(true);
        setDropIndicatorShown(true);
    }

    wxTextCtrl *GetEditControl()
    {
        return m_editorFactory.GetEditControl();
    }

    void SetItemImage(QTreeWidgetItem *item, int image, wxTreeItemIcon which)
    {
        m_imageStates[item][which] = image;
    }

    int GetItemImage(QTreeWidgetItem *item, wxTreeItemIcon which)
    {
        if (m_imageStates.find(item) == m_imageStates.end())
            return 0;

        return m_imageStates[item][which];
    }

protected:
    void drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const wxOVERRIDE
    {
        QTreeWidgetItem *item = itemFromIndex(index);

        QTreeWidget::drawBranches(painter, rect, index);
        const int imageIndex = ChooseBestImage(item);
        if (imageIndex != -1 )
        {
            wxImageList *imageList = GetHandler()->GetImageList();
            wxBitmap bitmap = imageList->GetBitmap(imageIndex);
            painter->drawPixmap(rect.topRight(), *bitmap.GetHandle());
        }
    }

private:
    void OnCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
    {
        wxTreeCtrl* treeCtrl = GetHandler();

        wxTreeEvent changingEvent(wxEVT_TREE_SEL_CHANGING, treeCtrl, wxQtConvertTreeItem(current));
        EmitEvent(changingEvent);

        if ( !changingEvent.IsAllowed() )
        {
            blockSignals(true);
            setCurrentItem(previous);
            blockSignals(false);
            return;
        }

        //QT doesnt update the selection until this singal has been processed.  //Defering this event ensures 
        //that wxTreeCtrl::GetSelection returns the new selection in the wx event handler.
        wxTreeEvent changedEvent(wxEVT_TREE_SEL_CHANGED, treeCtrl, wxQtConvertTreeItem(current));
        wxPostEvent(treeCtrl, changedEvent);
    }

    void OnItemActivated(QTreeWidgetItem *item, int WXUNUSED(column))
    {
        wxTreeEvent event(wxEVT_TREE_ITEM_ACTIVATED, GetHandler(), wxQtConvertTreeItem(item));
        EmitEvent(event);
    }

    void OnItemClicked(QTreeWidgetItem *item)
    {
        wxMouseState mouseState = wxGetMouseState();

        wxEventType eventType;
        if ( mouseState.RightIsDown() )
            eventType = wxEVT_TREE_ITEM_RIGHT_CLICK;
        else if ( mouseState.MiddleIsDown() )
            eventType = wxEVT_TREE_ITEM_MIDDLE_CLICK;
        else
            return;

        wxTreeEvent event(eventType, GetHandler(), wxQtConvertTreeItem(item));
        EmitEvent(event);
    }

    void OnItemCollapsed(QTreeWidgetItem *item)
    {
        wxTreeEvent collapsingEvent(wxEVT_TREE_ITEM_COLLAPSING, GetHandler(), wxQtConvertTreeItem(item));
        EmitEvent(collapsingEvent);

        if ( !collapsingEvent.IsAllowed() )
        {
            blockSignals(true);
            item->setExpanded(true);
            blockSignals(false);
            return;
        }

        wxTreeEvent collapsedEvent(wxEVT_TREE_ITEM_COLLAPSED, GetHandler(), wxQtConvertTreeItem(item));
        EmitEvent(collapsedEvent);
    }

    void OnItemExpanded(QTreeWidgetItem *item)
    {
        wxTreeEvent expandingEvent(wxEVT_TREE_ITEM_EXPANDING, GetHandler(), wxQtConvertTreeItem(item));
        EmitEvent(expandingEvent);

        if ( !expandingEvent.IsAllowed() )
        {
            blockSignals(true);
            item->setExpanded(false);
            blockSignals(false);
            return;
        }

        wxTreeEvent expandedEvent(wxEVT_TREE_ITEM_EXPANDED, GetHandler(), wxQtConvertTreeItem(item));
        EmitEvent(expandedEvent);
    }

    void OnItemChanged(QTreeWidgetItem *item, int WXUNUSED(column))
    {
        wxTreeEvent event(wxEVT_TREE_END_LABEL_EDIT, GetHandler(), wxQtConvertTreeItem(item));
        EmitEvent(event);
    }

    virtual void dragEnterEvent(QDragEnterEvent* event) wxOVERRIDE
    {
        wxEventType command = (event->mouseButtons() & Qt::RightButton)
            ? wxEVT_TREE_BEGIN_RDRAG
            : wxEVT_TREE_BEGIN_DRAG;


        QTreeWidgetItem *hitItem = itemAt(event->pos());

        wxTreeEvent tree_event(
            command,
            GetHandler(),
            wxQtConvertTreeItem(hitItem)
        );

        tree_event.SetPoint(wxQtConvertPoint(event->pos()));

        // Vetoed unless explicitly accepted.
        tree_event.Veto();

        EmitEvent(tree_event);

        if ( tree_event.IsAllowed() )
        {
            event->accept();
        }
    }

    virtual void dropEvent(QDropEvent* event) wxOVERRIDE
    {
        const wxPoint pos = wxQtConvertPoint(event->pos());
        QTreeWidgetItem *hitItem = itemAt(event->pos());

        wxTreeEvent tree_event(
            wxEVT_TREE_END_DRAG,
            GetHandler(),
            wxQtConvertTreeItem(hitItem)
        );

        tree_event.SetPoint(wxQtConvertPoint(event->pos()));

        EmitEvent(tree_event);
    }

    int ChooseBestImage(QTreeWidgetItem *item) const
    {
        int imageIndex = -1;

        const ImageStateMap::const_iterator i = m_imageStates.find(item);

        if (i == m_imageStates.end())
        {
            return -1;
        }

        const ImageState &states = i->second;

        if ( item->isExpanded() )
        {
            if ( item->isSelected() )
                imageIndex = states[wxTreeItemIcon_SelectedExpanded];

            if (imageIndex == -1)
                imageIndex = states[wxTreeItemIcon_Expanded];
        }
        else
        {
            if (item->isSelected())
                imageIndex = states[wxTreeItemIcon_Selected];
        }

        return QIcon::Normal;
    }

    wxQtTreeItemEditorFactory m_editorFactory;

    typedef std::map<QTreeWidgetItem*,ImageState> ImageStateMap;
    ImageStateMap m_imageStates;
};

wxTreeCtrl::wxTreeCtrl() :
    m_qtTreeWidget(NULL)
{
}

wxTreeCtrl::wxTreeCtrl(wxWindow *parent, wxWindowID id,
           const wxPoint& pos,
           const wxSize& size,
           long style,
           const wxValidator& validator,
           const wxString& name) 
{
    Create(parent, id, pos, size, style, validator, name);
}

bool wxTreeCtrl::Create(wxWindow *parent, wxWindowID id,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxValidator& validator,
            const wxString& name)
{
    m_qtTreeWidget = new wxQTreeWidget(parent, this);
    m_qtTreeWidget->header()->hide();

    SetWindowStyleFlag(style);

    return QtCreateControl(parent, id, pos, size, style, validator, name);
}

wxTreeCtrl::~wxTreeCtrl()
{
    if (m_qtTreeWidget != NULL)
        m_qtTreeWidget->deleteLater();
}

unsigned wxTreeCtrl::GetCount() const
{
    QTreeWidgetItem *root = m_qtTreeWidget->invisibleRootItem();
    if (root->childCount() == 0)
        return 0;

    return CountChildren(root->child(0));
}

unsigned wxTreeCtrl::GetIndent() const
{
    return m_qtTreeWidget->columnCount();
}

void wxTreeCtrl::SetIndent(unsigned int indent)
{
    m_qtTreeWidget->setColumnCount( indent );
}

void wxTreeCtrl::SetImageList(wxImageList *imageList)
{
    m_imageListNormal = imageList;

    int width, height;
    m_imageListNormal->GetSize(0, width, height);
    m_qtTreeWidget->setIconSize(QSize(width, height));
    m_qtTreeWidget->update();
}

void wxTreeCtrl::SetStateImageList(wxImageList *imageList)
{
    m_imageListState = imageList;
    m_qtTreeWidget->update();
}

wxString wxTreeCtrl::GetItemText(const wxTreeItemId& item) const
{
    if (!item.IsOk())
        return "";

    QTreeWidgetItem* qTreeItem = wxQtConvertTreeItem(item);
    return wxQtConvertString(qTreeItem->text(0));
}

int wxTreeCtrl::GetItemImage(const wxTreeItemId& item, wxTreeItemIcon which) const
{
    wxCHECK_MSG(item.IsOk(), -1, "invalid tree item");
    return m_qtTreeWidget->GetItemImage(wxQtConvertTreeItem(item), which);
}

wxTreeItemData *wxTreeCtrl::GetItemData(const wxTreeItemId& item) const
{
    wxCHECK_MSG(item.IsOk(), NULL, "invalid tree item");

    QTreeWidgetItem* qTreeItem = wxQtConvertTreeItem(item);
    QVariant itemData = qTreeItem->data(0, Qt::UserRole);
    TreeItemDataQt value = itemData.value<TreeItemDataQt>();
    return value.data;
}

wxColour wxTreeCtrl::GetItemTextColour(const wxTreeItemId& item) const
{
    wxCHECK_MSG(item.IsOk(), wxNullColour, "invalid tree item");

    QTreeWidgetItem* qTreeItem = wxQtConvertTreeItem(item);
    return wxQtConvertColour(qTreeItem->textColor(0));
}

wxColour wxTreeCtrl::GetItemBackgroundColour(const wxTreeItemId& item) const
{
    wxCHECK_MSG(item.IsOk(), wxNullColour, "invalid tree item");

    QTreeWidgetItem* qTreeItem = wxQtConvertTreeItem(item);
    return wxQtConvertColour(qTreeItem->backgroundColor(0));
}

wxFont wxTreeCtrl::GetItemFont(const wxTreeItemId& item) const
{
    wxCHECK_MSG(item.IsOk(), wxNullFont, "invalid tree item");

    QTreeWidgetItem* qTreeItem = wxQtConvertTreeItem(item);
    return wxFont(qTreeItem->font(0));
}

void wxTreeCtrl::SetItemText(const wxTreeItemId& item, const wxString& text)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");
    QTreeWidgetItem* qTreeItem = wxQtConvertTreeItem(item);
    qTreeItem->setText(0, wxQtConvertString(text));
}

void wxTreeCtrl::SetItemImage(const wxTreeItemId& item, int image, wxTreeItemIcon which)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");
    m_qtTreeWidget->SetItemImage(wxQtConvertTreeItem(item), image, which);
}

void wxTreeCtrl::SetItemData(const wxTreeItemId& item, wxTreeItemData *data)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    if (data != NULL)
        data->SetId(item);

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    TreeItemDataQt treeItemData(data);
    qTreeItem->setData(0, Qt::UserRole, QVariant::fromValue(treeItemData));
}

void wxTreeCtrl::SetItemHasChildren(const wxTreeItemId& item, bool has)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    qTreeItem->setChildIndicatorPolicy(has ? QTreeWidgetItem::ShowIndicator : QTreeWidgetItem::DontShowIndicatorWhenChildless);
}

void wxTreeCtrl::SetItemBold(const wxTreeItemId& item, bool bold)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    QFont font = qTreeItem->font(0);
    font.setBold(bold);
    qTreeItem->setFont(0, font);
}

void wxTreeCtrl::SetItemDropHighlight(const wxTreeItemId& item, bool highlight)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);

    wxColour fg, bg;

    if (highlight)
    {
        bg = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
        fg = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT);
    }
    else
    {
        bg = GetBackgroundColour();
        fg = GetForegroundColour();
    }

    qTreeItem->setBackgroundColor(0, wxQtConvertColour(bg));
    qTreeItem->setTextColor(0, wxQtConvertColour(fg));
}

void wxTreeCtrl::SetItemTextColour(const wxTreeItemId& item, const wxColour& col)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    qTreeItem->setTextColor(0, wxQtConvertColour(col));
}

void wxTreeCtrl::SetItemBackgroundColour(const wxTreeItemId& item, const wxColour& col)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    qTreeItem->setTextColor(0, wxQtConvertColour(col));
}

void wxTreeCtrl::SetItemFont(const wxTreeItemId& item, const wxFont& font)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    qTreeItem->setFont(0, font.GetHandle());
}

bool wxTreeCtrl::IsVisible(const wxTreeItemId& item) const
{
    wxCHECK_MSG(item.IsOk(), false, "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    const QRect visualRect = m_qtTreeWidget->visualItemRect(qTreeItem);
    return visualRect.isValid();
}

bool wxTreeCtrl::ItemHasChildren(const wxTreeItemId& item) const
{
    wxCHECK_MSG(item.IsOk(), false, "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    return qTreeItem->childCount() > 0;
}

bool wxTreeCtrl::IsExpanded(const wxTreeItemId& item) const
{
    wxCHECK_MSG(item.IsOk(), false, "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    return qTreeItem->isExpanded();
}

bool wxTreeCtrl::IsSelected(const wxTreeItemId& item) const
{
    wxCHECK_MSG(item.IsOk(), false, "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    return qTreeItem->isSelected();
}

bool wxTreeCtrl::IsBold(const wxTreeItemId& item) const
{
    wxCHECK_MSG(item.IsOk(), false, "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    QFont font = qTreeItem->font(0);
    return font.bold();
}

size_t wxTreeCtrl::GetChildrenCount(const wxTreeItemId& item, bool recursively) const
{
    wxCHECK_MSG(item.IsOk(), 0, "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);

    if (recursively)
        return CountChildren(qTreeItem);

    return qTreeItem->childCount();
}

wxTreeItemId wxTreeCtrl::GetRootItem() const
{
    QTreeWidgetItem *root = m_qtTreeWidget->invisibleRootItem();
    return wxQtConvertTreeItem(root->child(0));
}

wxTreeItemId wxTreeCtrl::GetSelection() const
{
    QList<QTreeWidgetItem*> selections = m_qtTreeWidget->selectedItems();
    return selections.isEmpty() ? wxTreeItemId() : wxQtConvertTreeItem(selections[0]);

}

size_t wxTreeCtrl::GetSelections(wxArrayTreeItemIds& selections) const
{
    QList<QTreeWidgetItem*> qtSelections = m_qtTreeWidget->selectedItems();

    const size_t numberOfSelections = qtSelections.size();
    for (size_t i = 0; i < numberOfSelections; ++i)
    {
        QTreeWidgetItem *item = qtSelections[i];
        selections.Add(wxQtConvertTreeItem(item));
    }

    return numberOfSelections;
}

void wxTreeCtrl::SetFocusedItem(const wxTreeItemId& item)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");
    m_qtTreeWidget->setCurrentItem(wxQtConvertTreeItem(item), 0);
}

void wxTreeCtrl::ClearFocusedItem()
{
    m_qtTreeWidget->setCurrentItem(NULL);
}

wxTreeItemId wxTreeCtrl::GetFocusedItem() const
{
    return wxQtConvertTreeItem(m_qtTreeWidget->currentItem());
}

wxTreeItemId wxTreeCtrl::GetItemParent(const wxTreeItemId& item) const
{
    wxCHECK_MSG(item.IsOk(), wxTreeItemId(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    return wxQtConvertTreeItem(qTreeItem->parent());
}

wxTreeItemId wxTreeCtrl::GetFirstChild(const wxTreeItemId& item, wxTreeItemIdValue& cookie) const
{
    wxCHECK_MSG(item.IsOk(), wxTreeItemId(), "invalid tree item");

    cookie = 0;
    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);

    return qTreeItem->childCount() > 0 ? wxQtConvertTreeItem(qTreeItem->child(0)) : wxTreeItemId();
}

wxTreeItemId wxTreeCtrl::GetNextChild(const wxTreeItemId& item, wxTreeItemIdValue& cookie) const
{
    wxCHECK_MSG(item.IsOk(), wxTreeItemId(), "invalid tree item");

    int currentIndex = reinterpret_cast<int>(cookie);
    ++currentIndex;

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);

    if ( currentIndex < qTreeItem->childCount() )
    {
        cookie = reinterpret_cast<wxTreeItemIdValue>(currentIndex);
        return wxQtConvertTreeItem(qTreeItem->child(currentIndex));
    }

    return wxTreeItemId();
}

wxTreeItemId wxTreeCtrl::GetLastChild(const wxTreeItemId& item) const
{
    wxCHECK_MSG(item.IsOk(), wxTreeItemId(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    const int childCount = qTreeItem->childCount();
    return childCount == 0 ? wxTreeItemId() : wxQtConvertTreeItem(qTreeItem->child(childCount - 1));
}

wxTreeItemId wxTreeCtrl::GetNextSibling(const wxTreeItemId& item) const
{
    wxCHECK_MSG(item.IsOk(), wxTreeItemId(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    QTreeWidgetItem *parent = qTreeItem->parent();

    if ( parent != NULL )
    {
        int index = parent->indexOfChild(qTreeItem);
        wxASSERT(index != -1);

        ++index;
        return index < parent->childCount() ? wxQtConvertTreeItem(parent->child(index)) : wxTreeItemId();
    }

    int index = m_qtTreeWidget->indexOfTopLevelItem(qTreeItem);
    wxASSERT(index != -1);

    ++index;
    return index < m_qtTreeWidget->topLevelItemCount() ? wxQtConvertTreeItem(m_qtTreeWidget->topLevelItem(index)) : wxTreeItemId();
}

wxTreeItemId wxTreeCtrl::GetPrevSibling(const wxTreeItemId& item) const
{
    wxCHECK_MSG(item.IsOk(), wxTreeItemId(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    QTreeWidgetItem *parent = qTreeItem->parent();

    if (parent != NULL)
    {
        int index = parent->indexOfChild(qTreeItem);
        wxASSERT(index != -1);

        --index;
        return index >= 0 ? wxQtConvertTreeItem(parent->child(index)) : wxTreeItemId();
    }

    int index = m_qtTreeWidget->indexOfTopLevelItem(qTreeItem);
    wxASSERT(index != -1);

    --index;
    return index >= 0 ? wxQtConvertTreeItem(m_qtTreeWidget->topLevelItem(index)) : wxTreeItemId();
}

wxTreeItemId wxTreeCtrl::GetFirstVisibleItem() const
{
   wxTreeItemId itemid = GetRootItem();
    if (!itemid.IsOk())
        return itemid;
    do
    {
        if (IsVisible(itemid))
            return itemid;
        itemid = GetNext(itemid);
    } while (itemid.IsOk());

    return wxTreeItemId();
}

wxTreeItemId wxTreeCtrl::GetNextVisible(const wxTreeItemId& item) const
{
    wxCHECK_MSG(item.IsOk(), wxTreeItemId(), wxT("invalid tree item"));
    wxASSERT_MSG(IsVisible(item), wxT("this item itself should be visible"));

    wxTreeItemId id = item;
    if (id.IsOk())
    {
        while (id = GetNext(id), id.IsOk())
        {
            if (IsVisible(id))
                return id;
        }
    }
    return wxTreeItemId();
}

wxTreeItemId wxTreeCtrl::GetPrevVisible(const wxTreeItemId& item) const
{
    wxCHECK_MSG(item.IsOk(), wxTreeItemId(), wxT("invalid tree item"));
    wxASSERT_MSG(IsVisible(item), wxT("this item itself should be visible"));

    // find out the starting point
    wxTreeItemId prevItem = GetPrevSibling(item);
    if (!prevItem.IsOk())
    {
        prevItem = GetItemParent(item);
    }

    // find the first visible item after it
    while (prevItem.IsOk() && !IsVisible(prevItem))
    {
        prevItem = GetNext(prevItem);
        if (!prevItem.IsOk() || prevItem == item)
        {
            // there are no visible items before item
            return wxTreeItemId();
        }
    }

    // from there we must be able to navigate until this item
    while (prevItem.IsOk())
    {
        const wxTreeItemId nextItem = GetNextVisible(prevItem);
        if (!nextItem.IsOk() || nextItem == item)
            break;

        prevItem = nextItem;
    }

    return prevItem;
}

wxTreeItemId wxTreeCtrl::AddRoot(const wxString& text,
                             int image, int selImage,
                             wxTreeItemData *data)
{
    QTreeWidgetItem *root = m_qtTreeWidget->invisibleRootItem();
    wxTreeItemId newItem =  DoInsertItem(wxQtConvertTreeItem(root), 0, text, image, selImage, data);
    m_qtTreeWidget->setCurrentItem(NULL);

    return newItem;
}

void wxTreeCtrl::Delete(const wxTreeItemId& item)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    QTreeWidgetItem *parent = qTreeItem->parent();

    if ( parent != NULL )
    {
        parent->removeChild(qTreeItem);
    }
    else
    {
        m_qtTreeWidget->removeItemWidget(qTreeItem, 0);
    }

    SendDeleteEvent(item);

    delete qTreeItem;
}

void wxTreeCtrl::DeleteChildren(const wxTreeItemId& item)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    while (qTreeItem->childCount() > 0)
    {
        QTreeWidgetItem *child = qTreeItem->child(0);
        DeleteChildren(wxQtConvertTreeItem(child));
        qTreeItem->removeChild(child);

        SendDeleteEvent(wxQtConvertTreeItem(child));

        delete child;
    }
}

void wxTreeCtrl::DeleteAllItems()
{
    DeleteChildren(wxQtConvertTreeItem(m_qtTreeWidget->invisibleRootItem()));
}

void wxTreeCtrl::Expand(const wxTreeItemId& item)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    qTreeItem->setExpanded(true);
}

void wxTreeCtrl::Collapse(const wxTreeItemId& item)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    qTreeItem->setExpanded(false);
}

void wxTreeCtrl::CollapseAndReset(const wxTreeItemId& item)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    Collapse(item);
    DeleteChildren(item);
}

void wxTreeCtrl::Toggle(const wxTreeItemId& item)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    qTreeItem->setSelected(!qTreeItem->isSelected());
}

void wxTreeCtrl::Unselect()
{
    QTreeWidgetItem *current = m_qtTreeWidget->currentItem();

    if (current != NULL)
        current->setSelected(false);
}

void wxTreeCtrl::UnselectAll()
{
    QList<QTreeWidgetItem *> selections = m_qtTreeWidget->selectedItems();
    const size_t selectedCount = selections.size();
    for ( size_t i = 0; i < selectedCount; ++i)
    {
        selections[i]->setSelected(false);
    }
}

void wxTreeCtrl::SelectItem(const wxTreeItemId& item, bool select)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    if ( !HasFlag(wxTR_MULTIPLE) )
    {
        QList<QTreeWidgetItem *> selections = m_qtTreeWidget->selectedItems();
        m_qtTreeWidget->clearSelection();
    }

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    qTreeItem->setSelected(select);
}

void wxTreeCtrl::SelectChildren(const wxTreeItemId& parent)
{
    wxCHECK_RET(parent.IsOk(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(parent);
    const int childCount = qTreeItem->childCount();

    for (int i = 0; i < childCount; ++i)
    {
        qTreeItem->child(i)->setSelected(true);
    }
}

void wxTreeCtrl::EnsureVisible(const wxTreeItemId& item)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    QTreeWidgetItem *parent = qTreeItem->parent();

    while ( parent != NULL )
    {
        parent->setExpanded(true);
        parent = parent->parent();
    }

    ScrollTo(item);
}

void wxTreeCtrl::ScrollTo(const wxTreeItemId& item)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    m_qtTreeWidget->scrollToItem(qTreeItem);
}

wxTextCtrl *wxTreeCtrl::EditLabel(const wxTreeItemId& item, wxClassInfo* WXUNUSED(textCtrlClass))
{
    wxCHECK_MSG(item.IsOk(), NULL, "invalid tree item");

    wxTreeEvent event(wxEVT_TREE_BEGIN_LABEL_EDIT, this, item);
    if ( HandleWindowEvent(event) && !event.IsAllowed() )
        return NULL;

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    m_qtTreeWidget->openPersistentEditor(qTreeItem);
    return m_qtTreeWidget->GetEditControl();
}

wxTextCtrl *wxTreeCtrl::GetEditControl() const
{
    return m_qtTreeWidget->GetEditControl();
}

void wxTreeCtrl::EndEditLabel(const wxTreeItemId& item, bool WXUNUSED(discardChanges))
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");
    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    m_qtTreeWidget->closePersistentEditor(qTreeItem);
}

void wxTreeCtrl::SortChildren(const wxTreeItemId& item)
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);
    qTreeItem->sortChildren(0, Qt::AscendingOrder);
}

bool wxTreeCtrl::GetBoundingRect(const wxTreeItemId& item, wxRect& WXUNUSED(rect), bool WXUNUSED(textOnly)) const
{
    wxCHECK_MSG(item.IsOk(), false, "invalid tree item");
    return false;
}

void wxTreeCtrl::SetWindowStyleFlag(long styles)
{
    wxControl::SetWindowStyleFlag(styles);
    m_qtTreeWidget->invisibleRootItem()->setHidden((styles & wxTR_HIDE_ROOT) != 0);
    m_qtTreeWidget->setSelectionMode(styles & wxTR_MULTIPLE ? QTreeWidget::ExtendedSelection : QTreeWidget::SingleSelection);
}

int wxTreeCtrl::DoGetItemState(const wxTreeItemId& item) const
{
    wxCHECK_MSG(item.IsOk(), wxTREE_ITEMSTATE_NONE, "invalid tree item");
    return 0;
}

void wxTreeCtrl::DoSetItemState(const wxTreeItemId& item, int WXUNUSED(state))
{
    wxCHECK_RET(item.IsOk(), "invalid tree item");
}

wxTreeItemId wxTreeCtrl::DoInsertItem(const wxTreeItemId& parent,
                                  size_t pos,
                                  const wxString& text,
                                  int image, int selImage,
                                  wxTreeItemData *data)
{
    wxCHECK_MSG(parent.IsOk(), wxTreeItemId(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(parent);

    QTreeWidgetItem *newItem = new QTreeWidgetItem;
    newItem->setText(0, wxQtConvertString(text));
    TreeItemDataQt treeItemData(data);

    newItem->setData(0, Qt::UserRole, QVariant::fromValue(treeItemData));

    if (pos == static_cast<size_t>(-1))
    {
        qTreeItem->addChild(newItem);
    }
    else
    {
        qTreeItem->insertChild(pos, newItem);
    }

    m_qtTreeWidget->SetItemImage(newItem, image, wxTreeItemIcon_Normal);
    m_qtTreeWidget->SetItemImage(newItem, selImage, wxTreeItemIcon_Selected);

    QPixmap pixmap(m_qtTreeWidget->iconSize());
    pixmap.fill(Qt::transparent);
    newItem->setIcon(0, pixmap);

    wxTreeItemId wxItem = wxQtConvertTreeItem(newItem);

    if (data != NULL)
        data->SetId(wxItem);

    return wxItem;
}

wxTreeItemId wxTreeCtrl::DoInsertAfter(const wxTreeItemId& parent,
                                   const wxTreeItemId& idPrevious,
                                   const wxString& text,
                                   int image, int selImage,
                                   wxTreeItemData *data)
{
    wxCHECK_MSG(parent.IsOk(), wxTreeItemId(), "invalid tree item");
    wxCHECK_MSG(idPrevious.IsOk(), wxTreeItemId(), "invalid tree item");

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(parent);
    const int index = qTreeItem->indexOfChild(wxQtConvertTreeItem(idPrevious));
    return DoInsertItem(parent, index + 1, text, image, selImage, data);
}

wxTreeItemId wxTreeCtrl::DoTreeHitTest(const wxPoint& point, int& flags) const
{
    int w, h;
    GetSize(&w, &h);
    flags = 0;
    if (point.x < 0) flags |= wxTREE_HITTEST_TOLEFT;
    if (point.x > w) flags |= wxTREE_HITTEST_TORIGHT;
    if (point.y < 0) flags |= wxTREE_HITTEST_ABOVE;
    if (point.y > h) flags |= wxTREE_HITTEST_BELOW;
    if (flags) return wxTreeItemId();

    QTreeWidgetItem *hitItem = m_qtTreeWidget->itemAt(wxQtConvertPoint(point));

    if (hitItem == NULL)
        flags |= wxTREE_HITTEST_NOWHERE;

    return wxQtConvertTreeItem(hitItem);
}

QWidget *wxTreeCtrl::GetHandle() const
{
    return m_qtTreeWidget;
}

void wxTreeCtrl::SendDeleteEvent(const wxTreeItemId &item)
{
    wxTreeEvent event(wxEVT_TREE_DELETE_ITEM, GetId());
    event.SetItem(item);
    HandleWindowEvent(event);
}

wxTreeItemId wxTreeCtrl::GetNext(const wxTreeItemId &item) const
{
    wxCHECK_MSG(item.IsOk(), wxTreeItemId(), wxT("invalid tree item"));

    QTreeWidgetItem *qTreeItem = wxQtConvertTreeItem(item);

    if ( qTreeItem->childCount() > 0)
    {
        return qTreeItem->child(0);
    }
    
    // Try a sibling of this or ancestor instead
    wxTreeItemId p = item;
    wxTreeItemId toFind;
    do
    {
        toFind = GetNextSibling(p);
        p = GetItemParent(p);
    } while (p.IsOk() && !toFind.IsOk());
    return toFind;
}

