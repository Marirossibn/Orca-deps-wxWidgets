#!################################################################################
#! File:    gtk.t
#! Purpose: tmake template file from which src/gtk/files.lst containing the
#!          list of files for wxGTK library is generated by tmake
#! Author:  Vadim Zeitlin
#! Created: 28.01.00
#! Version: $Id$
#!################################################################################
#${
    #! include the code which parses filelist.txt file and initializes
    #! %wxCommon, %wxGeneric, %wxHtml, %wxUNIX, %wxGTK, %wxMOTIF and
    #! %wxOS2PM hashes.
    IncludeTemplate("filelist.t");

    #! find all our sources
    $project{"COMMONOBJS"} .= "parser.o ";

    foreach $file (sort keys %wxGeneric) {
        next if $wxGeneric{$file} =~ /\bNotGTK\b/;

        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"GTK_SOURCES"} .= "generic/" . $file . " ";
        $project{"GUIOBJS"} .= $fileobj . " ";

        #! also add it to the list of object files used by wxUniv if there
        #! is no file with the same name among wxUniv own objects
        my $filereal = $file;
        if ( $file =~ /^([^.]+)g.cpp$/ ) {
            $filereal = "$1.cpp";
        }
        if ( !exists $wxUNIV{$filereal} ) {
            $project{"GUI_LOWLEVEL_OBJS"} .= $fileobj . " ";
        }
    }

    foreach $file (sort keys %wxCommon) {
        next if $wxCommon{$file} =~ /\bNotGTK\b/;

        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"GTK_SOURCES"} .= "common/" . $file . " ";
        $project{"COMMONOBJS"} .= $fileobj . " ";
    }

    foreach $file (sort keys %wxGTK) {
        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"GTK_SOURCES"} .= "gtk/" . $file . " ";
        $project{"GUIOBJS"} .= $fileobj . " ";

        if ( $wxGTK{$file} =~ /\bL\b/ ) {
            $project{"GUI_LOWLEVEL_OBJS"} .= $fileobj . " ";
        }
    }

    foreach $file (sort keys %wxUNIX) {
        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"GTK_SOURCES"} .= "unix/" . $file . " ";
        $project{"UNIXOBJS"} .= $fileobj . " ";
    }

    foreach $file (sort keys %wxHTML) {
        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"GTK_SOURCES"} .= "html/" . $file . " ";
        $project{"HTMLOBJS"} .= $fileobj . " ";
    }

    #! find all our headers
    foreach $file (sort keys %wxWXINCLUDE) {
        next if $wxWXINCLUDE{$file} =~ /\bNotGTK\b/;

        $project{"GTK_HEADERS"} .= $file . " "
    }

    foreach $file (sort keys %wxGTKINCLUDE) {
        $project{"GTK_HEADERS"} .= "gtk/" . $file . " "
    }

    foreach $file (sort keys %wxGENERICINCLUDE) {
        $project{"GTK_HEADERS"} .= "generic/" . $file . " "
    }

    foreach $file (sort keys %wxUNIXINCLUDE) {
        $project{"GTK_HEADERS"} .= "unix/" . $file . " "
    }

    foreach $file (sort keys %wxHTMLINCLUDE) {
        $project{"GTK_HEADERS"} .= "html/" . $file . " "
    }

    foreach $file (sort keys %wxPROTOCOLINCLUDE) {
        $project{"GTK_HEADERS"} .= "protocol/" . $file . " "
    }
#$}
# This file was automatically generated by tmake 
# DO NOT CHANGE THIS FILE, YOUR CHANGES WILL BE LOST! CHANGE GTK.T!
ALL_SOURCES = \
		#$ ExpandList("GTK_SOURCES");

ALL_HEADERS = \
		#$ ExpandList("GTK_HEADERS");

COMMONOBJS = \
		#$ ExpandList("COMMONOBJS");

GUIOBJS = \
		#$ ExpandList("GUIOBJS");

GUI_LOWLEVEL_OBJS = \
		#$ ExpandList("GUI_LOWLEVEL_OBJS");

UNIXOBJS = \
		#$ ExpandList("UNIXOBJS");

HTMLOBJS = \
		#$ ExpandList("HTMLOBJS");

