#!/bin/bash

echo "This script will create the gen_list contrib files in the current"
echo "directory.  Do you wish to continue? [y/n]"

typeset ans="n"
read ans

if [[ ${ans} != [Yy]* ]]; then
	echo "Goodbye."
	exit 1
fi


typeset fileList="Makefile README addr_clean.pl gen_list.c gen_list.h isa.c
isa.h main.c malloc_police.c malloc_police.h rec_char.c rec_char.h
rec_charptr.c rec_charptr.h rec_double.c rec_double.h rec_long.c
rec_long.h rec_malloc_police.c rec_malloc_police.h"


for i in ${fileList}; do

	echo "creating ${i}"

	####################################
	##  sed explained:
	##
	##		-e "s/^\(#${i}:\)\(.*\)/\2/"
	##			Find lines belonging to
	##			this file and remove the
	##			filename prefix.
	##
	##		-e "t print"
	##			Jump to label print if the
	##			above s command matched
	##
	##		-e "d"
	##			This command is skipped
	##			when the s command fails.
	##			This will remove any
	##			non-matching data.
	##
	##		-e ":print"
	##			The last thing sed does
	##			is print the data.  So,
	##			by now, we only print
	##			lines belonging to the file.
	##
	####################################

	sed	-e "s/^\(#${i}:\)\(.*\)/\2/" \
			-e "t print" \
			-e "d" \
			-e ":print" \
			${0} > ${i}

done

echo "done."

exit 0

#Makefile:PROGRAM	= test_list
#Makefile:CFLAGS	= -g
#Makefile:SRC		= main.c isa.c gen_list.c rec_char.c rec_charptr.c rec_double.c rec_long.c rec_malloc_police.c malloc_police.c
#Makefile:OBJS		= main.o isa.o gen_list.o rec_char.o rec_charptr.o rec_double.o rec_long.o rec_malloc_police.o malloc_police.o
#Makefile:
#Makefile:all : $(PROGRAM)
#Makefile:
#Makefile:$(PROGRAM) : $(OBJS)
#Makefile:	gcc $(CFLAGS) -o $(PROGRAM) $(OBJS)
#Makefile:
#Makefile:clean:
#Makefile:	rm -f $(OBJS)
#Makefile:
#Makefile:clobber: clean
#Makefile:	rm -f $(PROGRAM)


#README:This list supports:
#README:	copy construction,
#README:	"virtual" destruction,
#README:	streaming,
#README:	comparison (equal comparison currently supported).
#README:
#README:With the "object oriented" nature of the list, nodes, and records; it is
#README:easily concievable that sorted lists and hash tables could be implemented
#README:with little extra effort.
#README:
#README:
#README:Philosophical point:
#README:
#README:I am sure there is room for improvement with this design.  I am
#README:submitting this as a generic doubly linked list recomendation for IJB.
#README:Whatever the "collective" decides is fine with me.
#README:
#README:This implementation uses the "naming space" of gen_list, derived_rec,
#README:construct, copy construct, stream, destruct, etc...  These are open to
#README:argument.  I just used what was familiar to me and others in the "OO"
#README:community.  If these need changed to be adopted ... "so be it".
#README:
#README:
#README:Implementation point:
#README:
#README:I assume this is too late for a "3.0" release.  As I work for an
#README:airline, the whole past summer has been hectic (not to mention the
#README:last 4 months); but things have begun to settle down and I am
#README:following the IJB lists a bit more closely.  And I would like to say
#README:"HOLY CRAP!" .. you guys have accompolished a lot!  Way to go.
#README:
#README:But, the adoption of a better linked list package should at least be
#README:high on the next release list (if not the current one).  If you choose
#README:this submission or not, so be it.  But as a "data structure" man, I
#README:think IJB's linked lists need addressing.
#README:
#README:
#README:List/Enlist note:
#README:
#README:I have noticed the list.c file.  If this generic list is adopted, I
#README:think all existing functionallity could be duplicated with the
#README:"copy_contruct", "equal", and "destruct" `virtuals'.  This would also
#README:eliminate and/or enhance the other manually maintained lists in IJB.
#README:
#README:
#README:Debug note:
#README:
#README:Since the generic list defined a "stream" virtual, it could be programmed
#README:that the list could print itself whenever a FATAL error occurs.  A user (or
#README:programmer) could read the list and hopefully determine the cause of the
#README:abend.
#README:
#README:
#README:Potential note:
#README:
#README:Think of the possibilites of a linked list, sorted list, and/or a hash
#README:list.  Think of a request to a web site that has been referenced
#README:before.  If a hash list keep track of all block requests and regexp
#README:change commands, then a site could be blocked and/or modified without
#README:ever consulting the actions lists again.  What a speed up!
#README:
#README:What if some of the current lists were kept in sorted lists?  Then a
#README:search for a particular record could be a binary search instead of a
#README:linear search.
#README:
#README:The actions file(s) and regexp files(s) could be inserted into the
#README:list from front to back order (or visa versa) and the processing would
#README:take place in actual file order (which is more natural); rather than
#README:in reverse order (as it is today).
#README:
#README:
#README:Thank you for you time and attention to this contribution.  If it is
#README:"blessed" by the group, I am available to give time to integrating
#README:this into IJB.
#README:
#README:Let me know what y'all think about this package.
#README:
#README:-- 
#README:Rodney


#addr_clean.pl:#!/usr/bin/perl
#addr_clean.pl:
#addr_clean.pl:##	*********************************************************************
#addr_clean.pl:##	*
#addr_clean.pl:##	* File			:	$Source: /cvsroot/ijbswa/current/contrib.sh,v $
#addr_clean.pl:##	*
#addr_clean.pl:##	* Purpose		:	Cleans addresses out of ./test_list and replaces
#addr_clean.pl:##	*						them with "english" replacements.
#addr_clean.pl:##	*
#addr_clean.pl:##	* Usage			:	./test_list | ./addr_clean.pl
#addr_clean.pl:##	*
#addr_clean.pl:##	* Copyright		:	This program is free software; you can redistribute it 
#addr_clean.pl:##	*						and/or modify it under the terms of the GNU General
#addr_clean.pl:##	*						Public License as published by the Free Software
#addr_clean.pl:##	*						Foundation; either version 2 of the License, or (at
#addr_clean.pl:##	*						your option) any later version.
#addr_clean.pl:##	*
#addr_clean.pl:##	*						This program is distributed in the hope that it will
#addr_clean.pl:##	*						be useful, but WITHOUT ANY WARRANTY; without even the
#addr_clean.pl:##	*						implied warranty of MERCHANTABILITY or FITNESS FOR A
#addr_clean.pl:##	*						PARTICULAR PURPOSE.	See the GNU General Public
#addr_clean.pl:##	*						License for more details.
#addr_clean.pl:##	*
#addr_clean.pl:##	*						The GNU General Public License should be included with
#addr_clean.pl:##	*						this file.	If not, you can view it at
#addr_clean.pl:##	*						http://www.gnu.org/copyleft/gpl.html
#addr_clean.pl:##	*						or write to the Free Software Foundation, Inc., 59
#addr_clean.pl:##	*						Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#addr_clean.pl:##	*
#addr_clean.pl:##	**********************************************************************/
#addr_clean.pl:
#addr_clean.pl:use strict;
#addr_clean.pl:
#addr_clean.pl:
#addr_clean.pl:my $nMaxList = 0;
#addr_clean.pl:my $nMaxNode = 0;
#addr_clean.pl:my $nMaxRec = 0;
#addr_clean.pl:my %aaTranslation;
#addr_clean.pl:my $strLine;
#addr_clean.pl:
#addr_clean.pl:while ( $strLine = <STDIN> )
#addr_clean.pl:{
#addr_clean.pl:	if ( $strLine =~ m!(list.*=\s*)(0x[0-9a-f]+)! )
#addr_clean.pl:	{
#addr_clean.pl:		my $str1 = $1;
#addr_clean.pl:		my $str2 = $2;
#addr_clean.pl:
#addr_clean.pl:		if ( ! defined $aaTranslation{$str2} )
#addr_clean.pl:		{
#addr_clean.pl:			$aaTranslation{$str2} = "list" . ++ $nMaxList;
#addr_clean.pl:		}
#addr_clean.pl:		$strLine =~ s!(list.*=\s*)(0x[0-9a-f]+)!$str1($aaTranslation{$str2})!;
#addr_clean.pl:	}
#addr_clean.pl:
#addr_clean.pl:
#addr_clean.pl:	if ( $strLine =~ m!(node.*=\s*)(0x[0-9a-f]+)! )
#addr_clean.pl:	{
#addr_clean.pl:		my $str1 = $1;
#addr_clean.pl:		my $str2 = $2;
#addr_clean.pl:
#addr_clean.pl:		if ( ! defined $aaTranslation{$str2} )
#addr_clean.pl:		{
#addr_clean.pl:			$aaTranslation{$str2} = "node" . ++ $nMaxNode;
#addr_clean.pl:		}
#addr_clean.pl:		$strLine =~ s!(node.*=\s*)(0x[0-9a-f]+)!$str1($aaTranslation{$str2})!;
#addr_clean.pl:	}
#addr_clean.pl:
#addr_clean.pl:
#addr_clean.pl:	if ( $strLine =~ m!(rec.*=\s*(iter_. \.\.\. )?)(0x[0-9a-f]+)! )
#addr_clean.pl:	{
#addr_clean.pl:		my $str1 = $1;
#addr_clean.pl:		my $str2 = $3;
#addr_clean.pl:
#addr_clean.pl:		if ( ! defined $aaTranslation{$str2} )
#addr_clean.pl:		{
#addr_clean.pl:			$aaTranslation{$str2} = "rec" . ++ $nMaxRec;
#addr_clean.pl:		}
#addr_clean.pl:		$strLine =~ s!(rec.*=\s*(iter_. \.\.\. )?)(0x[0-9a-f]+)!$str1($aaTranslation{$str2})!;
#addr_clean.pl:	}
#addr_clean.pl:
#addr_clean.pl:
#addr_clean.pl:	## Catch the copy constuct syntax
#addr_clean.pl:
#addr_clean.pl:	if ( $strLine =~ m!(list.*=>\s*)(0x[0-9a-f]+)! )
#addr_clean.pl:	{
#addr_clean.pl:		my $str1 = $1;
#addr_clean.pl:		my $str2 = $2;
#addr_clean.pl:
#addr_clean.pl:		if ( ! defined $aaTranslation{$str2} )
#addr_clean.pl:		{
#addr_clean.pl:			$aaTranslation{$str2} = "list" . ++ $nMaxList;
#addr_clean.pl:		}
#addr_clean.pl:		$strLine =~ s!(list.*=>\s*)(0x[0-9a-f]+)!$str1($aaTranslation{$str2})!;
#addr_clean.pl:	}
#addr_clean.pl:
#addr_clean.pl:
#addr_clean.pl:	if ( $strLine =~ m!(node.*=>\s*)(0x[0-9a-f]+)! )
#addr_clean.pl:	{
#addr_clean.pl:		my $str1 = $1;
#addr_clean.pl:		my $str2 = $2;
#addr_clean.pl:
#addr_clean.pl:		if ( ! defined $aaTranslation{$str2} )
#addr_clean.pl:		{
#addr_clean.pl:			$aaTranslation{$str2} = "node" . ++ $nMaxNode;
#addr_clean.pl:		}
#addr_clean.pl:		$strLine =~ s!(node.*=>\s*)(0x[0-9a-f]+)!$str1($aaTranslation{$str2})!;
#addr_clean.pl:	}
#addr_clean.pl:
#addr_clean.pl:
#addr_clean.pl:	if ( $strLine =~ m!(rec.*=>\s*)(0x[0-9a-f]+)! )
#addr_clean.pl:	{
#addr_clean.pl:		my $str1 = $1;
#addr_clean.pl:		my $str2 = $2;
#addr_clean.pl:
#addr_clean.pl:		if ( ! defined $aaTranslation{$str2} )
#addr_clean.pl:		{
#addr_clean.pl:			$aaTranslation{$str2} = "rec" . ++ $nMaxRec;
#addr_clean.pl:		}
#addr_clean.pl:		$strLine =~ s!(rec.*=>\s*)(0x[0-9a-f]+)!$str1($aaTranslation{$str2})!;
#addr_clean.pl:	}
#addr_clean.pl:
#addr_clean.pl:	print( $strLine );
#addr_clean.pl:
#addr_clean.pl:}


#gen_list.c:const char gen_list_rcs[] = "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $";
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#gen_list.c: *
#gen_list.c: * Purpose     :  To create some functions to do generic doubly linked
#gen_list.c: *						list management.
#gen_list.c: *
#gen_list.c: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#gen_list.c: *                Privoxy team. http://www.privoxy.org/
#gen_list.c: *
#gen_list.c: *                This program is free software; you can redistribute it
#gen_list.c: *                and/or modify it under the terms of the GNU General
#gen_list.c: *                Public License as published by the Free Software
#gen_list.c: *                Foundation; either version 2 of the License, or (at
#gen_list.c: *                your option) any later version.
#gen_list.c: *
#gen_list.c: *                This program is distributed in the hope that it will
#gen_list.c: *                be useful, but WITHOUT ANY WARRANTY; without even the
#gen_list.c: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#gen_list.c: *                PARTICULAR PURPOSE.  See the GNU General Public
#gen_list.c: *                License for more details.
#gen_list.c: *
#gen_list.c: *                The GNU General Public License should be included with
#gen_list.c: *                this file.  If not, you can view it at
#gen_list.c: *                http://www.gnu.org/copyleft/gpl.html
#gen_list.c: *                or write to the Free Software Foundation, Inc., 59
#gen_list.c: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#gen_list.c: *
#gen_list.c: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#gen_list.c: *						and edit IJB, correctly.
#gen_list.c: *
#gen_list.c: * Revisions   :
#gen_list.c: *    $Log: contrib.sh,v $
#gen_list.c: *    Revision 1.3  2002/03/26 22:29:54  swa
#gen_list.c: *    we have a new homepage!
#gen_list.c: *
#gen_list.c: *    Revision 1.2  2002/03/24 13:25:43  swa
#gen_list.c: *    name change related issues
#gen_list.c: *
#gen_list.c: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#gen_list.c: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#gen_list.c: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#gen_list.c: *    contrib directory and I cannot upload a tarball ... it gets
#gen_list.c: *    corrupted).  This script will expand all files needed to create the
#gen_list.c: *    linked list modules and an example program.  Please see the README.
#gen_list.c: *    Feed back is welcomed.  Enjoy.
#gen_list.c: *
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:
#gen_list.c:
#gen_list.c:#include <malloc.h>
#gen_list.c:#include <stdio.h>
#gen_list.c:#include <stdlib.h>
#gen_list.c:#include <string.h>
#gen_list.c:
#gen_list.c:#include "gen_list.h"
#gen_list.c:#include "malloc_police.h"
#gen_list.c:
#gen_list.c:const char gen_list_h_rcs[] = GEN_LIST_H_VERSION;
#gen_list.c:
#gen_list.c:
#gen_list.c:/* This is used (for the moment) to cut out all the	*/
#gen_list.c:/* extra verbige of the malloc_police module.  After	*/
#gen_list.c:/* all, we do not want to see construct/destruct of a	*/
#gen_list.c:/* class that is supposed to be "in the background".	*/
#gen_list.c:
#gen_list.c:/* But it could lead to broader usage.						*/
#gen_list.c:
#gen_list.c:int list_is_quiet = 0;
#gen_list.c:
#gen_list.c:
#gen_list.c:#define CALL_VT_REC_COPY_CONSTRUCT(rec)	(((rec_copy_construct)(rec)->vtable[ VT_REC_COPY_CONSTRUCT ])( rec ))
#gen_list.c:#define CALL_VT_REC_DESTRUCT(rec)			(((rec_destruct)(rec)->vtable[ VT_REC_DESTRUCT ])( rec ))
#gen_list.c:#define CALL_VT_REC_STREAM(rec)				(((rec_stream)(rec)->vtable[ VT_REC_STREAM ])( rec ))
#gen_list.c:#define CALL_VT_REC_EQUAL(rec,eq_rec)		(((rec_equal)((rec)->vtable[ VT_REC_EQUAL ]))( rec, eq_rec ))
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_rec_construct
#gen_list.c: *
#gen_list.c: * Description :  Called from a derived list record class ONLY.
#gen_list.c: *						This function "construct" a classs: malloc_ed,
#gen_list.c: *						vtable, isa, and sizeof_rec "attributes".
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The record.  If NULL, malloc is called.
#gen_list.c: *				2	:	isa (prounced "is a") ... type of the record.
#gen_list.c: *				3	:	Memory image size of this record.
#gen_list.c: *			 ...	:	The "virtuals" for this record.  NOTE: this list
#gen_list.c: *						may increase if more "virtuals" are added.
#gen_list.c: *
#gen_list.c: * Returns     :  A pointer to the record (either the orig record
#gen_list.c: *						or the malloc_ed copy.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_rec *gen_list_rec_construct( enum GEN_LIST_ISA isa, int sizeof_rec, const t_vtable _vtable )
#gen_list.c:{
#gen_list.c:	struct gen_list_rec *this_rec = (struct gen_list_rec *)MALLOC( sizeof_rec );
#gen_list.c:	this_rec->vtable		= _vtable;
#gen_list.c:	this_rec->isa			= isa;
#gen_list.c:	this_rec->sizeof_rec	= sizeof_rec;
#gen_list.c:
#gen_list.c:	return( this_rec );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_rec_copy_construct
#gen_list.c: *
#gen_list.c: * Description :  Makes a copy of the current existing record.  All
#gen_list.c: *						inherited properties (isa, vtable, malloc_ed, ...)
#gen_list.c: *						are kept in tact after the copy.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  Existing record.
#gen_list.c: *				2	:  New record.
#gen_list.c: *
#gen_list.c: * Returns     :  The newly constructed copy record.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_rec *gen_list_rec_copy_construct( const struct gen_list_rec *this_rec )
#gen_list.c:{
#gen_list.c:	struct gen_list_rec *copy_rec = (struct gen_list_rec *)MALLOC( this_rec->sizeof_rec );
#gen_list.c:	copy_rec->vtable		= this_rec->vtable;
#gen_list.c:	copy_rec->isa			= this_rec->isa;
#gen_list.c:	copy_rec->sizeof_rec	= this_rec->sizeof_rec;
#gen_list.c:
#gen_list.c:	return( copy_rec );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_rec_destruct
#gen_list.c: *
#gen_list.c: * Description :  Destruct the record.  Including free_ing the memory
#gen_list.c: *						if applicable.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The record.
#gen_list.c: *
#gen_list.c: * Returns     :  NULL
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_rec *gen_list_rec_destruct( struct gen_list_rec *this_rec )
#gen_list.c:{
#gen_list.c:	FREE( this_rec );
#gen_list.c:	return( NULL );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_rec_stream
#gen_list.c: *
#gen_list.c: * Description :  Displays all attributes on the STDOUT stream.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The record.
#gen_list.c: *
#gen_list.c: * Returns     :  The record.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:const struct gen_list_rec *gen_list_rec_stream( const struct gen_list_rec *this_rec )
#gen_list.c:{
#gen_list.c:	LIST_SHOW( printf( "\t\tstream rec isa = %s\n", isa_ra[ this_rec->isa ] ) );
#gen_list.c:	return( this_rec );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_rec_equal
#gen_list.c: *
#gen_list.c: * Description :  Compares two records to see if they are equal.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  A record.
#gen_list.c: *          2  :  Another record.
#gen_list.c: *
#gen_list.c: * Returns     :  0 => NOT EQUAL, anything else is EQUAL.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:int gen_list_rec_equal( const struct gen_list_rec *this_rec, const struct gen_list_rec *eq_rec )
#gen_list.c:{
#gen_list.c:	if ( NULL == this_rec && NULL == eq_rec )
#gen_list.c:	{
#gen_list.c:		return( 1 );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	if (	( NULL == this_rec && NULL != eq_rec )	||
#gen_list.c:			( NULL != this_rec && NULL == eq_rec ) )
#gen_list.c:	{
#gen_list.c:		return( 0 );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	if (	( this_rec->isa != eq_rec->isa ) ||
#gen_list.c:			( this_rec->sizeof_rec != eq_rec->sizeof_rec ) )
#gen_list.c:	{
#gen_list.c:		LIST_SHOW( printf( "INFORMATION: sorry, but comparing different rec types is unsupported at this time.\n" ) );
#gen_list.c:		return( 0 );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	return( 1 );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/**********************************************************************/
#gen_list.c:
#gen_list.c:
#gen_list.c:struct gen_list_node
#gen_list.c:{
#gen_list.c:	/* private: */
#gen_list.c:	struct gen_list_node	*next;
#gen_list.c:	struct gen_list_node	*prev;
#gen_list.c:
#gen_list.c:	struct gen_list_rec	*rec;
#gen_list.c:};
#gen_list.c:/* public: */
#gen_list.c:extern struct gen_list_node *			gen_list_node_construct( struct gen_list_rec *_rec );
#gen_list.c:extern struct gen_list_node *			gen_list_node_copy_construct( const struct gen_list_node *this_node );
#gen_list.c:extern struct gen_list_node *			gen_list_node_destruct( struct gen_list_node *this_node );
#gen_list.c:extern struct gen_list_node *			gen_list_node_remove( struct gen_list_node *this_node );
#gen_list.c:
#gen_list.c:extern struct gen_list_node *			gen_list_node_set_next( struct gen_list_node *this_node, struct gen_list_node *_next );
#gen_list.c:extern struct gen_list_node *			gen_list_node_set_prev( struct gen_list_node *this_node, struct gen_list_node *_prev );
#gen_list.c:
#gen_list.c:extern struct gen_list_node *			gen_list_node_get_next( const struct gen_list_node *this_node );
#gen_list.c:extern struct gen_list_node *			gen_list_node_get_prev( const struct gen_list_node *this_node );
#gen_list.c:
#gen_list.c:extern const struct gen_list_rec	*	gen_list_node_stream( const struct gen_list_node *this_node );
#gen_list.c:extern int									gen_list_node_equal( const struct gen_list_node *this_node, const struct gen_list_node *eq_node );
#gen_list.c:
#gen_list.c:/* struct/class COMPLETE */
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_node_construct
#gen_list.c: *
#gen_list.c: * Description :  Constructs a generic list node and sets its record.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The node.  If NULL, malloc is called.
#gen_list.c: *          2  :  The nodes record contents.
#gen_list.c: *
#gen_list.c: * Returns     :  A pointer to the node (either the orig node
#gen_list.c: *						or the malloc_ed copy.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_node *gen_list_node_construct( struct gen_list_rec *_rec )
#gen_list.c:{
#gen_list.c:	struct gen_list_node *this_node = (struct gen_list_node *)MALLOC( sizeof( struct gen_list_node ) );
#gen_list.c:
#gen_list.c:	LIST_SHOW( printf( "\
#gen_list.c:\tconstruct new node\t\t= %p
#gen_list.c:\tconstruct new node->rec\t= %p\n\n", (const void *)this_node, (const void *)_rec ) );
#gen_list.c:
#gen_list.c:	this_node->rec		= _rec;
#gen_list.c:	this_node->next	= NULL;
#gen_list.c:	this_node->prev	= NULL;
#gen_list.c:
#gen_list.c:	return( this_node );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_node_copy_construct
#gen_list.c: *
#gen_list.c: * Description :  Makes a copy of the current node.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  Existing node.
#gen_list.c: *				2	:  New node.
#gen_list.c: *
#gen_list.c: * Returns     :  The newly constructed copy node.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_node *gen_list_node_copy_construct( const struct gen_list_node *this_node )
#gen_list.c:{
#gen_list.c:	struct gen_list_node *copy_node = (struct gen_list_node *)MALLOC( sizeof( struct gen_list_node ) );
#gen_list.c:
#gen_list.c:	LIST_SHOW( printf( "\tcopy construct new node = %p => %p\n\n", (const void *)this_node, (const void *)copy_node ) );
#gen_list.c:
#gen_list.c:	copy_node->rec		= CALL_VT_REC_COPY_CONSTRUCT( this_node->rec );
#gen_list.c:	copy_node->next	= NULL;
#gen_list.c:	copy_node->prev	= NULL;
#gen_list.c:
#gen_list.c:	return( copy_node );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_node_destruct
#gen_list.c: *
#gen_list.c: * Description :  Destruct the node.  Including free_ing the memory
#gen_list.c: *						if applicable.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The node.
#gen_list.c: *
#gen_list.c: * Returns     :  NULL
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_node *gen_list_node_destruct( struct gen_list_node *this_node )
#gen_list.c:{
#gen_list.c:	LIST_SHOW( printf( "\
#gen_list.c:\tdestruct this_node\t\t\t= %p
#gen_list.c:\tdestruct this_node->rec\t\t= %p
#gen_list.c:\tdestruct this_node->next\t= %p
#gen_list.c:\tdestruct this_node->prev\t= %p\n\n", (const void *)this_node, (const void *)this_node->rec, (const void *)this_node->next, (const void *)this_node->prev ) );
#gen_list.c:
#gen_list.c:	if ( NULL != this_node->rec )
#gen_list.c:	{
#gen_list.c:		CALL_VT_REC_DESTRUCT( this_node->rec );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	FREE( this_node );
#gen_list.c:	return( NULL );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_node_remove
#gen_list.c: *
#gen_list.c: * Description :  Destruct the node.  Including free_ing the memory
#gen_list.c: *						if applicable.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The node.
#gen_list.c: *
#gen_list.c: * Returns     :  NULL
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_node *gen_list_node_remove( struct gen_list_node *this_node )
#gen_list.c:{
#gen_list.c:	LIST_SHOW( printf( "\
#gen_list.c:\tremove this_node\t\t\t= %p
#gen_list.c:\tremove this_node->rec\t= %p
#gen_list.c:\tremove this_node->next\t= %p
#gen_list.c:\tremove this_node->prev\t= %p\n\n", (const void *)this_node, (const void *)this_node->rec, (const void *)this_node->next, (const void *)this_node->prev ) );
#gen_list.c:
#gen_list.c:	FREE( this_node );
#gen_list.c:	return( NULL );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_node_set_next
#gen_list.c: *
#gen_list.c: * Description :  Sets the next node in the list series.  Used mainly
#gen_list.c: *						when constructing and adding/removing nodes.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The node.
#gen_list.c: *          2  :  The next node.
#gen_list.c: *
#gen_list.c: * Returns     :  The node.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_node *gen_list_node_set_next( struct gen_list_node *this_node, struct gen_list_node *_next )
#gen_list.c:{
#gen_list.c:	this_node->next = _next;
#gen_list.c:	return( this_node );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_node_set_prev
#gen_list.c: *
#gen_list.c: * Description :  Sets the previous node in the list series.  Used mainly
#gen_list.c: *						when constructing and adding/removing nodes.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The node.
#gen_list.c: *          2  :  The previous node.
#gen_list.c: *
#gen_list.c: * Returns     :  The node.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_node *gen_list_node_set_prev( struct gen_list_node *this_node, struct gen_list_node *_prev )
#gen_list.c:{
#gen_list.c:	this_node->prev = _prev;
#gen_list.c:	return( this_node );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_node_get_next
#gen_list.c: *
#gen_list.c: * Description :  Gets the next node in the list series.  Used mainly
#gen_list.c: *						for traversing a list.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The node.
#gen_list.c: *
#gen_list.c: * Returns     :  The next node.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_node *gen_list_node_get_next( const struct gen_list_node *this_node )
#gen_list.c:{
#gen_list.c:	return( this_node->next );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_node_get_prev
#gen_list.c: *
#gen_list.c: * Description :  Gets the previous node in the list series.  Used mainly
#gen_list.c: *						for traversing a list.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The node.
#gen_list.c: *
#gen_list.c: * Returns     :  The previous node.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_node *gen_list_node_get_prev( const struct gen_list_node *this_node )
#gen_list.c:{
#gen_list.c:	return( this_node->prev );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_node_stream
#gen_list.c: *
#gen_list.c: * Description :  Displays all attributes on the STDOUT stream,
#gen_list.c: *						including the contents of the nodes record.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The record.
#gen_list.c: *
#gen_list.c: * Returns     :  The result of streaming the nodes record.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:const struct gen_list_rec *gen_list_node_stream( const struct gen_list_node *this_node )
#gen_list.c:{
#gen_list.c:	LIST_SHOW( printf( "\
#gen_list.c:\tstream this_node\t\t\t= %p
#gen_list.c:\tstream this_node->rec\t= %p
#gen_list.c:\tstream this_node->next\t= %p
#gen_list.c:\tstream this_node->prev\t= %p\n\n", (const void *)this_node, (const void *)this_node->rec, (const void *)this_node->next, (const void *)this_node->prev ) );
#gen_list.c:
#gen_list.c:	return( CALL_VT_REC_STREAM( this_node->rec ) );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_node_equal
#gen_list.c: *
#gen_list.c: * Description :  Compares two nodes (and their records) to see if they are equal.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  A node.
#gen_list.c: *          2  :  Another node.
#gen_list.c: *
#gen_list.c: * Returns     :  0 => NOT EQUAL, anything else is EQUAL.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:int gen_list_node_equal( const struct gen_list_node *this_node, const struct gen_list_node *eq_node )
#gen_list.c:{
#gen_list.c:	if ( NULL == this_node && NULL == eq_node )
#gen_list.c:	{
#gen_list.c:		return( 1 );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	if (	( NULL == this_node && NULL != eq_node ) ||
#gen_list.c:			( NULL != this_node && NULL == eq_node ) )
#gen_list.c:	{
#gen_list.c:		return( 0 );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	return( gen_list_rec_equal( this_node->rec, eq_node->rec ) );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/**********************************************************************/
#gen_list.c:
#gen_list.c:
#gen_list.c:/* private: */
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_insert_node
#gen_list.c: *
#gen_list.c: * Description :  Inserts a node (not a record!) into a list.  This
#gen_list.c: *						is (currently) only used in copy construction.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The list.
#gen_list.c: *          2  :  A new node to insert into the list.
#gen_list.c: *
#gen_list.c: * Returns     :  The node.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:const struct gen_list_node *gen_list_insert_node( struct gen_list *this_list, struct gen_list_node *node )
#gen_list.c:{
#gen_list.c:	gen_list_node_set_next( node, NULL );
#gen_list.c:
#gen_list.c:	if ( NULL == this_list->first )
#gen_list.c:	{
#gen_list.c:		this_list->first = node;
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	if ( NULL == this_list->last )
#gen_list.c:	{
#gen_list.c:		this_list->last = node;
#gen_list.c:	}
#gen_list.c:	else
#gen_list.c:	{
#gen_list.c:		gen_list_node_set_next( this_list->last, node );
#gen_list.c:		gen_list_node_set_prev( node, this_list->last );
#gen_list.c:		this_list->last = node;
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	this_list->last = node;
#gen_list.c:
#gen_list.c:	return( node );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_construct
#gen_list.c: *
#gen_list.c: * Description :  Constructs a generic list.
#gen_list.c: *
#gen_list.c: * Parameters  :	None
#gen_list.c: *
#gen_list.c: * Returns     :  A pointer to the list (either the orig list
#gen_list.c: *						or the malloc_ed copy.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list *gen_list_construct()
#gen_list.c:{
#gen_list.c:	struct gen_list *this_list = (struct gen_list *)MALLOC( sizeof( struct gen_list ) );
#gen_list.c:	LIST_SHOW( printf( "construct new list = %p\n\n", (const void *)this_list ) );
#gen_list.c:
#gen_list.c:	this_list->first	= NULL;
#gen_list.c:	this_list->last	= NULL;
#gen_list.c:
#gen_list.c:	return( this_list );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_copy_construct
#gen_list.c: *
#gen_list.c: * Description :  Makes a copy of the current list.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  Existing list.
#gen_list.c: *				2	:  New list.
#gen_list.c: *
#gen_list.c: * Returns     :  The newly constructed copy list.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list *gen_list_copy_construct( const struct gen_list *this_list )
#gen_list.c:{
#gen_list.c:	struct gen_list_node *curr = this_list->first;
#gen_list.c:	struct gen_list *copy_list = (struct gen_list *)MALLOC( sizeof( struct gen_list ) );
#gen_list.c:
#gen_list.c:	LIST_SHOW( printf( "copy construct new list = %p => %p\n\n", (const void *)this_list, (const void *)copy_list ) );
#gen_list.c:
#gen_list.c:	copy_list->first	= NULL;
#gen_list.c:	copy_list->last	= NULL;
#gen_list.c:
#gen_list.c:
#gen_list.c:	while ( NULL != curr )
#gen_list.c:	{
#gen_list.c:		struct gen_list_node *copy_node = gen_list_node_copy_construct( curr );
#gen_list.c:		gen_list_insert_node( copy_list, copy_node );
#gen_list.c:		curr = gen_list_node_get_next( curr );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	return( copy_list );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_destruct
#gen_list.c: *
#gen_list.c: * Description :  Destruct the list.  Including free_ing the memory
#gen_list.c: *						if applicable.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The list.
#gen_list.c: *
#gen_list.c: * Returns     :  NULL
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list *gen_list_destruct( struct gen_list *this_list )
#gen_list.c:{
#gen_list.c:	struct gen_list_node *curr = this_list->first;
#gen_list.c:
#gen_list.c:	LIST_SHOW( printf( "\
#gen_list.c:destruct this_list\t\t\t= %p
#gen_list.c:destruct this_list->first\t= %p
#gen_list.c:destruct this_list->last\t= %p\n\n", (const void *)this_list, (const void *)this_list->first, (const void *)this_list->last ) );
#gen_list.c:
#gen_list.c:	while ( NULL != curr )
#gen_list.c:	{
#gen_list.c:		struct gen_list_node *next = gen_list_node_get_next( curr );
#gen_list.c:		gen_list_node_destruct( curr );
#gen_list.c:		curr = next;
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	FREE( this_list );
#gen_list.c:	return( NULL );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_remove_all
#gen_list.c: *
#gen_list.c: * Description :  Destruct the list.  Including free_ing the memory
#gen_list.c: *						if applicable.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The list.
#gen_list.c: *
#gen_list.c: * Returns     :  NULL
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list *gen_list_remove_all( struct gen_list *this_list )
#gen_list.c:{
#gen_list.c:	struct gen_list_node *curr = this_list->first;
#gen_list.c:
#gen_list.c:	LIST_SHOW( printf( "\
#gen_list.c:remove_all this_list\t\t\t\t= %p
#gen_list.c:remove_all this_list->first\t= %p
#gen_list.c:remove_all this_list->last\t\t= %p\n\n", (const void *)this_list, (const void *)this_list->first, (const void *)this_list->last ) );
#gen_list.c:
#gen_list.c:	while ( NULL != curr )
#gen_list.c:	{
#gen_list.c:		struct gen_list_node *next = gen_list_node_get_next( curr );
#gen_list.c:		gen_list_node_remove( curr );
#gen_list.c:		curr = next;
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	this_list->first = this_list->last = NULL;
#gen_list.c:	return( this_list );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_remove
#gen_list.c: *
#gen_list.c: * Description :  A record from the list and return it if found.  This
#gen_list.c: *						will allow a futher call to rec_destruct.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The list.
#gen_list.c: *          2  :  The record to be removed.
#gen_list.c: *
#gen_list.c: * Returns     :  rec if found and removed, NULL otherwise.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_rec *gen_list_remove( struct gen_list *this_list, struct gen_list_rec *rec )
#gen_list.c:{
#gen_list.c:	struct gen_list_node	*curr = this_list->first;
#gen_list.c:
#gen_list.c:	LIST_SHOW( printf( "\
#gen_list.c:remove this_list\t\t\t= %p
#gen_list.c:remove this_list->first\t= %p
#gen_list.c:remove this_list->last\t= %p
#gen_list.c:remove rec\t\t\t\t\t= %p\n\n", (const void *)this_list, (const void *)this_list->first, (const void *)this_list->last, rec ) );
#gen_list.c:
#gen_list.c:	while ( NULL != curr )
#gen_list.c:	{
#gen_list.c:		/* Grab these values before the are destroyed. */
#gen_list.c:		struct gen_list_node	*prev = gen_list_node_get_prev( curr );
#gen_list.c:		struct gen_list_node	*next = gen_list_node_get_next( curr );
#gen_list.c:
#gen_list.c:		/* Access to rec, next, prev of gen_list_node is allowed since it is private to gen_list */
#gen_list.c:		if ( curr->rec == rec )
#gen_list.c:		{
#gen_list.c:			/* Destruct the node, but not the record. */
#gen_list.c:			gen_list_node_remove( curr );
#gen_list.c:
#gen_list.c:			if ( NULL == prev )
#gen_list.c:			{
#gen_list.c:				/************************************************/
#gen_list.c:				/* This is the first node in the list.				*/
#gen_list.c:				/* 	Picture:												*/
#gen_list.c:				/* [first] ---------------------> [next]			*/
#gen_list.c:				/* [NULL] <---------------------- [next->prev]	*/
#gen_list.c:				/************************************************/
#gen_list.c:				this_list->first = next;
#gen_list.c:
#gen_list.c:				if ( NULL == next )
#gen_list.c:				{
#gen_list.c:					/* Since next is NULL, then this is an empty list. */
#gen_list.c:					this_list->last = NULL;
#gen_list.c:				}
#gen_list.c:				else
#gen_list.c:				{
#gen_list.c:					/* Since next is not NULL, then there is another node to make first. */
#gen_list.c:					gen_list_node_set_prev( next, NULL );
#gen_list.c:				}
#gen_list.c:			}
#gen_list.c:			else if ( NULL == next )
#gen_list.c:			{
#gen_list.c:				/************************************************/
#gen_list.c:				/* This is the last node in the list.				*/
#gen_list.c:				/* 	Picture:												*/
#gen_list.c:				/* [last] ----------------------> [prev]			*/
#gen_list.c:				/* [prev->next] ----------------> [NULL]			*/
#gen_list.c:				/************************************************/
#gen_list.c:				this_list->last = prev;
#gen_list.c:
#gen_list.c:				if ( NULL == prev )
#gen_list.c:				{
#gen_list.c:					/* Since prev is NULL, then this is an empty list. */
#gen_list.c:					this_list->first = NULL;
#gen_list.c:				}
#gen_list.c:				else
#gen_list.c:				{
#gen_list.c:					/* Since prev is NULL, then there is another node to make last. */
#gen_list.c:					gen_list_node_set_next( prev, NULL );
#gen_list.c:				}
#gen_list.c:			}
#gen_list.c:			else
#gen_list.c:			{
#gen_list.c:				/************************************************/
#gen_list.c:				/* This is a middle node in the list.				*/
#gen_list.c:				/* 	Picture:												*/
#gen_list.c:				/* [prev->next] ----------------> [next]			*/
#gen_list.c:				/*					[removing middle]						*/
#gen_list.c:				/* [prev] <---------------------- [next->prev]	*/
#gen_list.c:				/************************************************/
#gen_list.c:				gen_list_node_set_next( prev, next );
#gen_list.c:				gen_list_node_set_prev( next, prev );
#gen_list.c:			}
#gen_list.c:
#gen_list.c:			return( rec );
#gen_list.c:		}
#gen_list.c:		curr = next;
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	return( NULL );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_get_first
#gen_list.c: *
#gen_list.c: * Description :  Gets the first record in the list.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The list.
#gen_list.c: *
#gen_list.c: * Returns     :  The first node.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_rec *gen_list_get_first( const struct gen_list *this_list )
#gen_list.c:{
#gen_list.c:	if ( NULL == this_list->first )
#gen_list.c:	{
#gen_list.c:		return( NULL );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	/* Access to rec of gen_list_node is allowed since it is private to gen_list */
#gen_list.c:	return( this_list->first->rec );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_get_last
#gen_list.c: *
#gen_list.c: * Description :  Gets the last record in the list.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The list.
#gen_list.c: *
#gen_list.c: * Returns     :  The last node.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_rec *gen_list_get_last( const struct gen_list *this_list )
#gen_list.c:{
#gen_list.c:	if ( NULL == this_list->last )
#gen_list.c:	{
#gen_list.c:		return( NULL );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	/* Access to rec of gen_list_node is allowed since it is private to gen_list */
#gen_list.c:	return( this_list->last->rec );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_insert
#gen_list.c: *
#gen_list.c: * Description :  Inserts a record into the list.  This is the
#gen_list.c: *						primary API interface for inserting a record.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The list.
#gen_list.c: *          2  :  A new record to insert into the list.
#gen_list.c: *
#gen_list.c: * Returns     :  The node.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:const struct gen_list_node *gen_list_insert( struct gen_list *this_list, struct gen_list_rec *_rec )
#gen_list.c:{
#gen_list.c:	return( gen_list_insert_node( this_list, gen_list_node_construct( _rec ) ) );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_stream
#gen_list.c: *
#gen_list.c: * Description :  Displays all attributes on the STDOUT stream,
#gen_list.c: *						including the contents of the nodes.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The list.
#gen_list.c: *
#gen_list.c: * Returns     :  The list.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:const struct gen_list *gen_list_stream( const struct gen_list *this_list )
#gen_list.c:{
#gen_list.c:	const struct gen_list_node *curr = this_list->first;
#gen_list.c:
#gen_list.c:	LIST_SHOW( printf( "\
#gen_list.c:stream this_list\t\t\t= %p
#gen_list.c:stream this_list->first\t= %p
#gen_list.c:stream this_list->last\t= %p\n\n", (const void *)this_list, (const void *)this_list->first, (const void *)this_list->last ) );
#gen_list.c:
#gen_list.c:	while ( NULL != curr )
#gen_list.c:	{
#gen_list.c:		struct gen_list_node	*next = gen_list_node_get_next( curr );
#gen_list.c:		gen_list_node_stream( curr );
#gen_list.c:		curr = next;
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	return( this_list );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_equal
#gen_list.c: *
#gen_list.c: * Description :  Compares two lists (and their nodes) to see if they are equal.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  A list.
#gen_list.c: *          2  :  Another list.
#gen_list.c: *
#gen_list.c: * Returns     :  0 => NOT EQUAL, anything else is EQUAL.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:int gen_list_equal( const struct gen_list *this_list, const struct gen_list *eq_list )
#gen_list.c:{
#gen_list.c:	struct gen_list_node *c1 = this_list->first;
#gen_list.c:	struct gen_list_node *c2 = eq_list->first;
#gen_list.c:	int eq = 1;
#gen_list.c:
#gen_list.c:	if ( ( NULL == c1 && NULL != c2 ) || ( NULL != c1 && NULL == c2 ) )
#gen_list.c:	{
#gen_list.c:		return( 0 );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	while ( ( eq ) && ( NULL != c1 ) && ( NULL != c2 ) )
#gen_list.c:	{
#gen_list.c:		/* Access to rec of gen_list_node is allowed since it is private to gen_list */
#gen_list.c:		eq = CALL_VT_REC_EQUAL( c1->rec, c2->rec );
#gen_list.c:		if ( NULL != c1 )
#gen_list.c:		{
#gen_list.c:			c1 = gen_list_node_get_next( c1 );
#gen_list.c:		}
#gen_list.c:
#gen_list.c:		if ( NULL != c2 )
#gen_list.c:		{
#gen_list.c:			c2 = gen_list_node_get_next( c2 );
#gen_list.c:		}
#gen_list.c:
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	if ( NULL == c1 && NULL == c2 )
#gen_list.c:	{
#gen_list.c:		return( 1 );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	return( eq );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_find
#gen_list.c: *
#gen_list.c: * Description :  Find a record that matches the one passed to this
#gen_list.c: *						function.
#gen_list.c: *						NOTE: this function receives a RECORD and not a NODE.
#gen_list.c: *						Some implementation issues might be easier addressed
#gen_list.c: *						if a node was passed ... but for the time being, I
#gen_list.c: *						would like to keep the use/programmer isolated from
#gen_list.c: *						the node structure/class.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The list.
#gen_list.c: *          2  :  The record to find.
#gen_list.c: *
#gen_list.c: * Returns     :  NULL => none found, anything else is a pointer to the record.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_rec *gen_list_find( const struct gen_list *this_list, struct gen_list_rec *rec )
#gen_list.c:{
#gen_list.c:	const struct gen_list_node *curr = this_list->first;
#gen_list.c:	int eq = 0;
#gen_list.c:
#gen_list.c:	if ( NULL == curr && NULL == rec )
#gen_list.c:	{
#gen_list.c:		return( (struct gen_list_rec *)1 );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	if (	( NULL == curr && NULL != rec )	||
#gen_list.c:			( NULL != curr && NULL == rec ) )
#gen_list.c:	{
#gen_list.c:		return( 0 );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	/* Access to rec of gen_list_node is allowed since it is private to gen_list */
#gen_list.c:	if ( curr->rec->isa != rec->isa )
#gen_list.c:	{
#gen_list.c:		LIST_SHOW( printf( "INFORMATION: sorry, but comparing different list types is unsupported at this time.\n" ) );
#gen_list.c:		return( 0 );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	if ( NULL == curr )
#gen_list.c:	{
#gen_list.c:		return( NULL );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:
#gen_list.c:	while ( ( !eq ) && ( NULL != curr ) )
#gen_list.c:	{
#gen_list.c:		struct gen_list_node	*next = gen_list_node_get_next( curr );
#gen_list.c:
#gen_list.c:		eq = CALL_VT_REC_EQUAL( curr->rec, rec );
#gen_list.c:
#gen_list.c:		if ( eq )
#gen_list.c:		{
#gen_list.c:			return( curr->rec );
#gen_list.c:		}
#gen_list.c:
#gen_list.c:		curr = next;
#gen_list.c:
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	return( NULL );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_iterate
#gen_list.c: *
#gen_list.c: * Description :  Pass over the list, calling "iter" for every record.
#gen_list.c: *						If the "iter" function returns a non-NULL value,
#gen_list.c: *						return it and stop iterating.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The list.
#gen_list.c: *          2  :  The iterator function.
#gen_list.c: *
#gen_list.c: * Returns     :  Whatever the iterator does, or NULL if the entire
#gen_list.c: *						list was processed.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_rec *gen_list_iterate( struct gen_list *this_list, rec_iterate iter )
#gen_list.c:{
#gen_list.c:	struct gen_list_node *curr = this_list->first;
#gen_list.c:
#gen_list.c:	if ( NULL == curr )
#gen_list.c:	{
#gen_list.c:		return( NULL );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	while ( NULL != curr )
#gen_list.c:	{
#gen_list.c:		/* Access to rec of gen_list_node is allowed since it is private to gen_list */
#gen_list.c:		struct gen_list_node	*next = gen_list_node_get_next( curr );
#gen_list.c:		struct gen_list_rec *r = iter( curr->rec );
#gen_list.c:
#gen_list.c:		if ( NULL != r )
#gen_list.c:		{
#gen_list.c:			return( r );
#gen_list.c:		}
#gen_list.c:
#gen_list.c:		curr = next;
#gen_list.c:
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	return( NULL );
#gen_list.c:
#gen_list.c:}
#gen_list.c:
#gen_list.c:
#gen_list.c:/*********************************************************************
#gen_list.c: *
#gen_list.c: * Function    :  gen_list_iterate
#gen_list.c: *
#gen_list.c: * Description :  Pass over the list (in reverse order), calling "iter"
#gen_list.c: *						for every record.  If the "iter" function returns
#gen_list.c: *						a non-NULL value, return it and stop iterating.
#gen_list.c: *
#gen_list.c: * Parameters  :
#gen_list.c: *          1  :  The list.
#gen_list.c: *          2  :  The iterator function.
#gen_list.c: *
#gen_list.c: * Returns     :  Whatever the iterator does, or NULL if the entire
#gen_list.c: *						list was processed.
#gen_list.c: *
#gen_list.c: *********************************************************************/
#gen_list.c:struct gen_list_rec *gen_list_iterate_reverse( struct gen_list *this_list, rec_iterate iter )
#gen_list.c:{
#gen_list.c:	struct gen_list_node *curr = this_list->last;
#gen_list.c:
#gen_list.c:	if ( NULL == curr )
#gen_list.c:	{
#gen_list.c:		return( NULL );
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	while ( NULL != curr )
#gen_list.c:	{
#gen_list.c:		/* Access to rec of gen_list_node is allowed since it is private to gen_list */
#gen_list.c:		struct gen_list_node	*prev = gen_list_node_get_prev( curr );
#gen_list.c:		struct gen_list_rec *r = iter( curr->rec );
#gen_list.c:
#gen_list.c:		if ( NULL != r )
#gen_list.c:		{
#gen_list.c:			return( r );
#gen_list.c:		}
#gen_list.c:
#gen_list.c:		curr = prev;
#gen_list.c:
#gen_list.c:	}
#gen_list.c:
#gen_list.c:	return( NULL );
#gen_list.c:
#gen_list.c:}


#gen_list.h:#ifndef GEN_LIST_H_INCLUDED
#gen_list.h:#define GEN_LIST_H_INCLUDED
#gen_list.h:#define GEN_LIST_H_VERSION "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $"
#gen_list.h:/*********************************************************************
#gen_list.h: *
#gen_list.h: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#gen_list.h: *
#gen_list.h: * Purpose     :  To create some functions to do generic doubly linked
#gen_list.h: *						list management.
#gen_list.h: *
#gen_list.h: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#gen_list.h: *                Privoxy team. http://www.privoxy.org/
#gen_list.h: *
#gen_list.h: *                This program is free software; you can redistribute it
#gen_list.h: *                and/or modify it under the terms of the GNU General
#gen_list.h: *                Public License as published by the Free Software
#gen_list.h: *                Foundation; either version 2 of the License, or (at
#gen_list.h: *                your option) any later version.
#gen_list.h: *
#gen_list.h: *                This program is distributed in the hope that it will
#gen_list.h: *                be useful, but WITHOUT ANY WARRANTY; without even the
#gen_list.h: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#gen_list.h: *                PARTICULAR PURPOSE.  See the GNU General Public
#gen_list.h: *                License for more details.
#gen_list.h: *
#gen_list.h: *                The GNU General Public License should be included with
#gen_list.h: *                this file.  If not, you can view it at
#gen_list.h: *                http://www.gnu.org/copyleft/gpl.html
#gen_list.h: *                or write to the Free Software Foundation, Inc., 59
#gen_list.h: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#gen_list.h: *
#gen_list.h: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#gen_list.h: *						and edit IJB, correctly.
#gen_list.h: *
#gen_list.h: * Revisions   :
#gen_list.h: *    $Log: contrib.sh,v $
#gen_list.h: *    Revision 1.3  2002/03/26 22:29:54  swa
#gen_list.h: *    we have a new homepage!
#gen_list.h: *
#gen_list.h: *    Revision 1.2  2002/03/24 13:25:43  swa
#gen_list.h: *    name change related issues
#gen_list.h: *
#gen_list.h: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#gen_list.h: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#gen_list.h: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#gen_list.h: *    contrib directory and I cannot upload a tarball ... it gets
#gen_list.h: *    corrupted).  This script will expand all files needed to create the
#gen_list.h: *    linked list modules and an example program.  Please see the README.
#gen_list.h: *    Feed back is welcomed.  Enjoy.
#gen_list.h: *
#gen_list.h: *
#gen_list.h: *********************************************************************/
#gen_list.h:
#gen_list.h:
#gen_list.h:#ifdef __cplusplus
#gen_list.h:extern "C" {
#gen_list.h:#endif
#gen_list.h:
#gen_list.h:#include "isa.h"
#gen_list.h:
#gen_list.h:
#gen_list.h:extern int list_is_quiet;
#gen_list.h:
#gen_list.h:#define LIST_SHOW(stmt)  if ( ! list_is_quiet ) { stmt; }
#gen_list.h:
#gen_list.h:
#gen_list.h:struct gen_list_rec;
#gen_list.h:typedef struct gen_list_rec *			(*rec_copy_construct)( struct gen_list_rec *copy_rec );
#gen_list.h:typedef struct gen_list_rec *			(*rec_destruct)( struct gen_list_rec *this_rec );
#gen_list.h:typedef const struct gen_list_rec *	(*rec_stream)( const struct gen_list_rec *this_rec );
#gen_list.h:typedef int									(*rec_equal)( const struct gen_list_rec *this_rec, const struct gen_list_rec *eq_rec );
#gen_list.h:typedef struct gen_list_rec *			(*rec_iterate)( struct gen_list_rec *this_rec );
#gen_list.h:
#gen_list.h:typedef void * (*rec_method)( void *this_rec, ... );
#gen_list.h:
#gen_list.h:
#gen_list.h:enum VT_ENTRIES
#gen_list.h:{
#gen_list.h:	VT_REC_COPY_CONSTRUCT,
#gen_list.h:	VT_REC_DESTRUCT,
#gen_list.h:	VT_REC_STREAM,
#gen_list.h:	VT_REC_EQUAL,
#gen_list.h:	VT_REC_MAX
#gen_list.h:};
#gen_list.h:
#gen_list.h:
#gen_list.h:typedef const rec_method *t_vtable;
#gen_list.h:
#gen_list.h:
#gen_list.h:struct gen_list_rec
#gen_list.h:{
#gen_list.h:	/* private: */
#gen_list.h:	t_vtable vtable;
#gen_list.h:	enum GEN_LIST_ISA isa;
#gen_list.h:	int sizeof_rec;
#gen_list.h:	/* contents HERE */
#gen_list.h:};
#gen_list.h:/* public: */
#gen_list.h:extern struct gen_list_rec *			gen_list_rec_construct( enum GEN_LIST_ISA isa, int sizeof_rec, const t_vtable _vtable );
#gen_list.h:extern struct gen_list_rec *			gen_list_rec_copy_construct( const struct gen_list_rec *this_rec );
#gen_list.h:extern struct gen_list_rec *			gen_list_rec_destruct( struct gen_list_rec *this_rec );
#gen_list.h:extern const struct gen_list_rec *	gen_list_rec_stream( const struct gen_list_rec *this_rec );
#gen_list.h:extern int									gen_list_rec_equal( const struct gen_list_rec *this_rec, const struct gen_list_rec *eq_rec );
#gen_list.h:
#gen_list.h:/* struct/class COMPLETE */
#gen_list.h:
#gen_list.h:
#gen_list.h:/**********************************************************************/
#gen_list.h:
#gen_list.h:/* private: to gen_list */
#gen_list.h:struct gen_list_node;
#gen_list.h:
#gen_list.h:
#gen_list.h:struct gen_list
#gen_list.h:{
#gen_list.h:	/* private: */
#gen_list.h:	struct gen_list_node *first;
#gen_list.h:	struct gen_list_node *last;
#gen_list.h:};
#gen_list.h:/* gen_list_insert_node */
#gen_list.h:
#gen_list.h:/* public: */
#gen_list.h:extern struct gen_list *				gen_list_construct();
#gen_list.h:extern struct gen_list *				gen_list_copy_construct( const struct gen_list *this_list );
#gen_list.h:extern struct gen_list *				gen_list_destruct( struct gen_list *this_list );
#gen_list.h:extern struct gen_list *				gen_list_remove_all( struct gen_list *this_list );
#gen_list.h:extern struct gen_list_rec *			gen_list_remove( struct gen_list *this_list, struct gen_list_rec *_rec );
#gen_list.h:extern struct gen_list_rec *			gen_list_get_first( const struct gen_list *this_list );
#gen_list.h:extern struct gen_list_rec *			gen_list_get_last( const struct gen_list *this_list );
#gen_list.h:extern const struct gen_list_node *	gen_list_insert( struct gen_list *this_list, struct gen_list_rec *_rec );
#gen_list.h:extern const struct gen_list *		gen_list_stream( const struct gen_list *this_list );
#gen_list.h:extern int									gen_list_equal( const struct gen_list *this_list, const struct gen_list *eq_list );
#gen_list.h:extern struct gen_list_rec *			gen_list_find( const struct gen_list *this_list, struct gen_list_rec *rec );
#gen_list.h:extern struct gen_list_rec *			gen_list_iterate( struct gen_list *this_list, rec_iterate iter );
#gen_list.h:/* struct/class COMPLETE */
#gen_list.h:
#gen_list.h:
#gen_list.h:#ifdef __cplusplus
#gen_list.h:} /* extern "C" */
#gen_list.h:#endif
#gen_list.h:
#gen_list.h:#endif /* ndef GEN_LIST_H_INCLUDED */
#gen_list.h:
#gen_list.h:/*
#gen_list.h:  Local Variables:
#gen_list.h:  tab-width: 3
#gen_list.h:  end:
#gen_list.h:*/


#isa.c:const char isa_rcs[] = "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $";
#isa.c:/*********************************************************************
#isa.c: *
#isa.c: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#isa.c: *
#isa.c: * Purpose     :  Pronounced "is a".  To create "english" translations
#isa.c: *						for all linked "classes".
#isa.c: *
#isa.c: *						NOTE:	this header file must appear once and only once
#isa.c: *								per project.
#isa.c: *
#isa.c: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#isa.c: *                Privoxy team. http://www.privoxy.org/
#isa.c: *
#isa.c: *                This program is free software; you can redistribute it
#isa.c: *                and/or modify it under the terms of the GNU General
#isa.c: *                Public License as published by the Free Software
#isa.c: *                Foundation; either version 2 of the License, or (at
#isa.c: *                your option) any later version.
#isa.c: *
#isa.c: *                This program is distributed in the hope that it will
#isa.c: *                be useful, but WITHOUT ANY WARRANTY; without even the
#isa.c: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#isa.c: *                PARTICULAR PURPOSE.  See the GNU General Public
#isa.c: *                License for more details.
#isa.c: *
#isa.c: *                The GNU General Public License should be included with
#isa.c: *                this file.  If not, you can view it at
#isa.c: *                http://www.gnu.org/copyleft/gpl.html
#isa.c: *                or write to the Free Software Foundation, Inc., 59
#isa.c: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#isa.c: *
#isa.c: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#isa.c: *						and edit IJB, correctly.
#isa.c: *
#isa.c: * Revisions   :
#isa.c: *    $Log: contrib.sh,v $
#isa.c: *    Revision 1.3  2002/03/26 22:29:54  swa
#isa.c: *    we have a new homepage!
#isa.c: *
#isa.c: *    Revision 1.2  2002/03/24 13:25:43  swa
#isa.c: *    name change related issues
#isa.c: *
#isa.c: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#isa.c: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#isa.c: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#isa.c: *    contrib directory and I cannot upload a tarball ... it gets
#isa.c: *    corrupted).  This script will expand all files needed to create the
#isa.c: *    linked list modules and an example program.  Please see the README.
#isa.c: *    Feed back is welcomed.  Enjoy.
#isa.c: *
#isa.c: *    Revision 1.1  2001/11/30 21:55:51  rodney
#isa.c: *    Initial revision
#isa.c: *
#isa.c: *
#isa.c: *********************************************************************/
#isa.c:
#isa.c:
#isa.c:#include <malloc.h>
#isa.c:#include <stdio.h>
#isa.c:#include <stdlib.h>
#isa.c:#include <string.h>
#isa.c:
#isa.c:#include "gen_list.h"
#isa.c:#include "isa.h"
#isa.c:
#isa.c:const char isa_h_rcs[] = ISA_H_VERSION;
#isa.c:
#isa.c:
#isa.c:/* Pronounce "ra" phonectically and you get "array", which this variable is. */
#isa.c:char *isa_ra[] =
#isa.c:{
#isa.c:	"GEN_LIST_REC",
#isa.c:	"REC_MALLOC_POLICE",
#isa.c:	"REC_CHAR",
#isa.c:	"REC_CHARPTR",
#isa.c:	"REC_DOUBLE",
#isa.c:	"REC_LONG",
#isa.c:	NULL
#isa.c:};


#isa.h:#ifndef ISA_H_INCLUDED
#isa.h:#define ISA_H_INCLUDED
#isa.h:#define ISA_H_VERSION "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $"
#isa.h:/*********************************************************************
#isa.h: *
#isa.h: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#isa.h: *
#isa.h: * Purpose     :  Pronounced "is a".  To create "english" translations
#isa.h: *						for all linked "classes".
#isa.h: *
#isa.h: *						NOTE:	this header file must appear once and only once
#isa.h: *								per project.
#isa.h: *
#isa.h: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#isa.h: *                Privoxy team. http://www.privoxy.org/
#isa.h: *
#isa.h: *                This program is free software; you can redistribute it
#isa.h: *                and/or modify it under the terms of the GNU General
#isa.h: *                Public License as published by the Free Software
#isa.h: *                Foundation; either version 2 of the License, or (at
#isa.h: *                your option) any later version.
#isa.h: *
#isa.h: *                This program is distributed in the hope that it will
#isa.h: *                be useful, but WITHOUT ANY WARRANTY; without even the
#isa.h: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#isa.h: *                PARTICULAR PURPOSE.  See the GNU General Public
#isa.h: *                License for more details.
#isa.h: *
#isa.h: *                The GNU General Public License should be included with
#isa.h: *                this file.  If not, you can view it at
#isa.h: *                http://www.gnu.org/copyleft/gpl.html
#isa.h: *                or write to the Free Software Foundation, Inc., 59
#isa.h: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#isa.h: *
#isa.h: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#isa.h: *						and edit IJB, correctly.
#isa.h: *
#isa.h: * Revisions   :
#isa.h: *    $Log: contrib.sh,v $
#isa.h: *    Revision 1.3  2002/03/26 22:29:54  swa
#isa.h: *    we have a new homepage!
#isa.h: *
#isa.h: *    Revision 1.2  2002/03/24 13:25:43  swa
#isa.h: *    name change related issues
#isa.h: *
#isa.h: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#isa.h: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#isa.h: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#isa.h: *    contrib directory and I cannot upload a tarball ... it gets
#isa.h: *    corrupted).  This script will expand all files needed to create the
#isa.h: *    linked list modules and an example program.  Please see the README.
#isa.h: *    Feed back is welcomed.  Enjoy.
#isa.h: *
#isa.h: *
#isa.h: *********************************************************************/
#isa.h:
#isa.h:
#isa.h:#ifdef __cplusplus
#isa.h:extern "C" {
#isa.h:#endif
#isa.h:
#isa.h:
#isa.h:enum GEN_LIST_ISA
#isa.h:{
#isa.h:	ISA_GEN_LIST_REC,
#isa.h:	ISA_MALLOC_POLICE,
#isa.h:	ISA_CHAR,
#isa.h:	ISA_CHARPTR,
#isa.h:	ISA_DOUBLE,
#isa.h:	ISA_LONG,
#isa.h:	ISA_MAX
#isa.h:};
#isa.h:
#isa.h:
#isa.h:/* Pronounce "ra" phonectically and you get "array", which this variable is. */
#isa.h:extern char *isa_ra[];
#isa.h:
#isa.h:
#isa.h:#ifdef __cplusplus
#isa.h:} /* extern "C" */
#isa.h:#endif
#isa.h:
#isa.h:#endif /* ndef ISA_H_INCLUDED */
#isa.h:
#isa.h:/*
#isa.h:  Local Variables:
#isa.h:  tab-width: 3
#isa.h:  end:
#isa.h:*/


#main.c:const char main_rcs[] = "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $";
#main.c:/*********************************************************************
#main.c: *
#main.c: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#main.c: *
#main.c: * Purpose     :  To test "generic list" creation and manipulation.
#main.c: *
#main.c: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#main.c: *                Privoxy team. http://www.privoxy.org/
#main.c: *
#main.c: *                This program is free software; you can redistribute it
#main.c: *                and/or modify it under the terms of the GNU General
#main.c: *                Public License as published by the Free Software
#main.c: *                Foundation; either version 2 of the License, or (at
#main.c: *                your option) any later version.
#main.c: *
#main.c: *                This program is distributed in the hope that it will
#main.c: *                be useful, but WITHOUT ANY WARRANTY; without even the
#main.c: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#main.c: *                PARTICULAR PURPOSE.  See the GNU General Public
#main.c: *                License for more details.
#main.c: *
#main.c: *                The GNU General Public License should be included with
#main.c: *                this file.  If not, you can view it at
#main.c: *                http://www.gnu.org/copyleft/gpl.html
#main.c: *                or write to the Free Software Foundation, Inc., 59
#main.c: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#main.c: *
#main.c: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#main.c: *						and edit IJB, correctly.
#main.c: *
#main.c: * Note			:	This output assumes (per IJB standards) that tabs are
#main.c: *						set to 3 characters.  If you run this in a terminal,
#main.c: *						you may want to run this as so:
#main.c: *							>./test_list 2>&1 | expand -t3 | ./addr_clean.pl
#main.c: *						This will expand the tab to 3 columns and then replace
#main.c: *						"0x" style addresses with more human readable ones.
#main.c: *
#main.c: * Revisions   :
#main.c: *    $Log: contrib.sh,v $
#main.c: *    Revision 1.3  2002/03/26 22:29:54  swa
#main.c: *    we have a new homepage!
#main.c: *
#main.c: *    Revision 1.2  2002/03/24 13:25:43  swa
#main.c: *    name change related issues
#main.c: *
#main.c: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#main.c: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#main.c: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#main.c: *    contrib directory and I cannot upload a tarball ... it gets
#main.c: *    corrupted).  This script will expand all files needed to create the
#main.c: *    linked list modules and an example program.  Please see the README.
#main.c: *    Feed back is welcomed.  Enjoy.
#main.c: *
#main.c: *
#main.c: *********************************************************************/
#main.c:
#main.c:
#main.c:#include <malloc.h>
#main.c:#include <stdio.h>
#main.c:#include <stdlib.h>
#main.c:#include <string.h>
#main.c:
#main.c:#include "gen_list.h"
#main.c:#include "rec_char.h"
#main.c:#include "rec_charptr.h"
#main.c:#include "rec_double.h"
#main.c:#include "rec_long.h"
#main.c:#include "malloc_police.h"
#main.c:
#main.c:
#main.c:extern const char isa_rcs[];
#main.c:extern const char isa_h_rcs[];
#main.c:extern const char gen_list_rcs[];
#main.c:extern const char gen_list_h_rcs[];
#main.c:extern const char malloc_police_rcs[];
#main.c:extern const char malloc_police_h_rcs[];
#main.c:extern const char rec_charptr_rcs[];
#main.c:extern const char rec_charptr_h_rcs[];
#main.c:extern const char rec_long_rcs[];
#main.c:extern const char rec_long_h_rcs[];
#main.c:extern const char rec_double_rcs[];
#main.c:extern const char rec_double_h_rcs[];
#main.c:extern const char rec_char_rcs[];
#main.c:extern const char rec_char_h_rcs[];
#main.c:extern const char rec_malloc_police_rcs[];
#main.c:extern const char rec_malloc_police_h_rcs[];
#main.c:extern const char main_rcs[];
#main.c:
#main.c:
#main.c:/*********************************************************************
#main.c: *
#main.c: * Function    :  my_custom_charptr_iterator_1
#main.c: *
#main.c: * Description :  This funtion is called once for every record in a
#main.c: *						list.  Or at least until we return non-NULL.  BTW,
#main.c: *						this function always returns NULL, thus it will
#main.c: *						iterate over the entire list.
#main.c: *
#main.c: * Parameters  :
#main.c: *          1  :  The record.
#main.c: *
#main.c: * Returns     :  NULL
#main.c: *
#main.c: *********************************************************************/
#main.c:struct derived_rec_charptr *my_custom_charptr_iterator_1( struct derived_rec_charptr *this_rec )
#main.c:{
#main.c:	printf( "\
#main.c:\t\tcharptr iterator_1 this_rec\t\t\t\t= iter_1 ... %p
#main.c:\t\tcharptr iterator_1 this_rec->contents\t= iter_1 ... %s\n\n", (const void *)this_rec, this_rec->contents );
#main.c:	return( NULL );
#main.c:
#main.c:}
#main.c:
#main.c:
#main.c:/*********************************************************************
#main.c: *
#main.c: * Function    :  my_custom_charptr_iterator_2
#main.c: *
#main.c: * Description :  This funtion is called once for every record in a
#main.c: *						list.  Or at least until we return non-NULL.  BTW,
#main.c: *						this function always returns non-NULL, thus it will
#main.c: *						iterate over only 1 record.
#main.c: *
#main.c: * Parameters  :
#main.c: *          1  :  The record.
#main.c: *
#main.c: * Returns     :  The record.
#main.c: *
#main.c: *********************************************************************/
#main.c:struct derived_rec_charptr *my_custom_charptr_iterator_2( struct derived_rec_charptr *this_rec )
#main.c:{
#main.c:	printf( "\
#main.c:\t\tcharptr iterator_1 this_rec\t\t\t\t= iter_2 ... %p
#main.c:\t\tcharptr iterator_1 this_rec->contents\t= iter_2 ... %s\n\n", (const void *)this_rec, this_rec->contents );
#main.c:	return( this_rec );
#main.c:
#main.c:}
#main.c:
#main.c:
#main.c:/*********************************************************************
#main.c: *
#main.c: * Function    :  main
#main.c: *
#main.c: * Description :  Test the doubly linked list as it is so far.
#main.c: *
#main.c: * Parameters  :  None
#main.c: *
#main.c: * Returns     :  N/A
#main.c: *
#main.c: *********************************************************************/
#main.c:int main( int argc, const char **argv )
#main.c:{
#main.c:	struct gen_list *lcharptr;
#main.c:	struct gen_list *llong;
#main.c:	struct gen_list *ldouble;
#main.c:	struct gen_list *lchar;
#main.c:
#main.c:	struct gen_list *lcharptr_c;
#main.c:	struct gen_list *llong_c;
#main.c:	struct gen_list *ldouble_c;
#main.c:	struct gen_list *lchar_c;
#main.c:
#main.c:	struct gen_list_rec *f_rec;
#main.c:
#main.c:	struct gen_list *no_own_list;
#main.c:	struct gen_list_rec *no_own_rec1;
#main.c:	struct gen_list_rec *no_own_rec2;
#main.c:	struct gen_list_rec *no_own_rec3;
#main.c:
#main.c:	printf( "\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
#main.c:			  gen_list_rcs,
#main.c:			  gen_list_h_rcs,
#main.c:			  isa_rcs,
#main.c:			  isa_h_rcs,
#main.c:			  malloc_police_rcs,
#main.c:			  malloc_police_h_rcs,
#main.c:			  rec_char_rcs,
#main.c:			  rec_char_h_rcs,
#main.c:			  rec_charptr_rcs,
#main.c:			  rec_charptr_h_rcs,
#main.c:			  rec_long_rcs,
#main.c:			  rec_long_h_rcs,
#main.c:			  rec_double_rcs,
#main.c:			  rec_double_h_rcs,
#main.c:			  rec_malloc_police_rcs,
#main.c:			  rec_malloc_police_h_rcs,
#main.c:			  main_rcs );
#main.c:
#main.c:
#main.c:	/* Force a memory leak to test the strdup/malloc/free police. */
#main.c:	/* NOTE: this should trigger the alphanum check of allocated memory. */
#main.c:	strcpy( MALLOC( 60 ), "Force a memory leak to test the strdup/malloc/free police." );
#main.c:
#main.c:
#main.c:	printf( "** -- Calling gen_list_construct to constuct a charptr list -- **\n" );
#main.c:	lcharptr = gen_list_construct();
#main.c:
#main.c:	gen_list_insert( lcharptr, (struct gen_list_rec *)derived_rec_charptr_construct( "Rodney Stromlund" ) );
#main.c:	gen_list_insert( lcharptr, f_rec = (struct gen_list_rec *)derived_rec_charptr_construct( "RJS" ) );
#main.c:	gen_list_insert( lcharptr, (struct gen_list_rec *)derived_rec_charptr_construct( "jammin!" ) );
#main.c:
#main.c:	printf( "** -- Calling gen_list_stream to test streaming a charptr list -- **\n" );
#main.c:	gen_list_stream( lcharptr );
#main.c:
#main.c:	printf( "** -- Calling gen_list_find to find the 2nd charptr record -- **\n" );
#main.c:	f_rec = gen_list_find( lcharptr, f_rec );
#main.c:	printf( "Found rec = %p\n", f_rec );
#main.c:	derived_rec_charptr_stream( (const struct derived_rec_charptr *)f_rec );
#main.c:
#main.c:
#main.c:	printf( "** -- Calling gen_list_copy_construct to test copy construct the charptr list -- **\n" );
#main.c:	lcharptr_c = gen_list_copy_construct( lcharptr );
#main.c:
#main.c:	printf( "** -- Calling gen_list_equal to see if the copied list is the same as the orig -- **
#main.c:** -- NOTE: they will NOT be because the charptr copy constructor changes the string contents -- **\n" );
#main.c:	printf( "Are these 2 equal? => %d\n\n", gen_list_equal( lcharptr, lcharptr_c ) );
#main.c:
#main.c:	printf( "** -- Calling gen_list_stream to stream a copied charptr list -- **\n" );
#main.c:	gen_list_stream( lcharptr_c );
#main.c:
#main.c:
#main.c:	printf( "** -- Calling gen_list_construct to constuct a long list -- **\n" );
#main.c:	llong = gen_list_construct();
#main.c:
#main.c:
#main.c:	gen_list_insert( llong, no_own_rec1 = (struct gen_list_rec *)derived_rec_long_construct( 1 ) );
#main.c:	gen_list_insert( llong, no_own_rec2 = (struct gen_list_rec *)derived_rec_long_construct( 100 ) );
#main.c:	gen_list_insert( llong, no_own_rec3 = (struct gen_list_rec *)derived_rec_long_construct( 200 ) );
#main.c:
#main.c:	printf( "** -- Calling gen_list_stream to test streaming a long list -- **\n" );
#main.c:	gen_list_stream( llong );
#main.c:
#main.c:
#main.c:	printf( "** -- Calling gen_list_copy_construct to test copy construct the long list -- **\n" );
#main.c:	llong_c = gen_list_copy_construct( llong );
#main.c:
#main.c:	printf( "** -- Calling gen_list_stream to stream a copied long list -- **\n" );
#main.c:	gen_list_stream( llong_c );
#main.c:
#main.c:	printf( "** -- Calling gen_list_construct to test a no-owner long list -- **\n" );
#main.c:	no_own_list = gen_list_construct();
#main.c:	gen_list_insert( no_own_list, no_own_rec1 );
#main.c:	gen_list_insert( no_own_list, no_own_rec2 );
#main.c:	gen_list_insert( no_own_list, no_own_rec3 );
#main.c:
#main.c:	printf( "** -- Calling gen_list_stream to stream a no-owner list -- **\n" );
#main.c:	gen_list_stream( no_own_list );
#main.c:
#main.c:	printf( "** -- Calling gen_list_equal to see if list and no-owner list are equal  NOTE: they should be. -- **\n" );
#main.c:	printf( "Are these 2 equal? => %d\n\n", gen_list_equal( llong, no_own_list ) );
#main.c:
#main.c:
#main.c:	printf( "** -- Calling gen_list_construct to constuct a double list -- **\n" );
#main.c:	ldouble = gen_list_construct();
#main.c:
#main.c:	gen_list_insert( ldouble, (struct gen_list_rec *)derived_rec_double_construct( 3.0 ) );
#main.c:	gen_list_insert( ldouble, (struct gen_list_rec *)derived_rec_double_construct( 3.1 ) );
#main.c:	gen_list_insert( ldouble, (struct gen_list_rec *)derived_rec_double_construct( 3.14 ) );
#main.c:
#main.c:	printf( "** -- Calling gen_list_stream to test streaming a double list -- **\n" );
#main.c:	gen_list_stream( ldouble );
#main.c:
#main.c:
#main.c:	printf( "** -- Calling gen_list_copy_construct to test copy construct the double list -- **\n" );
#main.c:	ldouble_c = gen_list_copy_construct( ldouble );
#main.c:
#main.c:	printf( "** -- Calling gen_list_stream to stream a copied double list -- **\n" );
#main.c:	gen_list_stream( ldouble_c );
#main.c:
#main.c:
#main.c:	printf( "** -- Calling gen_list_construct to constuct a char list -- **\n" );
#main.c:	lchar = gen_list_construct();
#main.c:
#main.c:	gen_list_insert( lchar, (struct gen_list_rec *)derived_rec_char_construct( '1' ) );
#main.c:	gen_list_insert( lchar, (struct gen_list_rec *)derived_rec_char_construct( 'A' ) );
#main.c:	gen_list_insert( lchar, (struct gen_list_rec *)derived_rec_char_construct( 'a' ) );
#main.c:
#main.c:	printf( "** -- Calling gen_list_stream to test streaming a char list -- **\n" );
#main.c:	gen_list_stream( lchar );
#main.c:
#main.c:
#main.c:	printf( "** -- Calling gen_list_copy_construct to test copy construct the char list -- **\n" );
#main.c:	lchar_c = gen_list_copy_construct( lchar );
#main.c:
#main.c:	printf( "** -- Calling gen_list_equal to see if the copied list is the same as the orig  NOTE: they should be. -- **\n" );
#main.c:	printf( "Are these 2 equal? => %d\n\n", gen_list_equal( lchar, lchar_c ) );
#main.c:
#main.c:	printf( "** -- Calling gen_list_stream to stream a copied char list -- **\n" );
#main.c:	gen_list_stream( lchar_c );
#main.c:
#main.c:
#main.c:	printf( "** -- Calling gen_list_iterate to iterate a charptr list -- **
#main.c:** -- NOTE: this iterator (my_custom_charptr_iterator_1) will iterate the whole list. -- **\n" );
#main.c:	gen_list_iterate( lcharptr,	(rec_iterate)my_custom_charptr_iterator_1 );
#main.c:
#main.c:	printf( "** -- Calling gen_list_iterate to iterate a copied charptr list -- **
#main.c:** -- NOTE: this iterator (my_custom_charptr_iterator_2) will return after the first iteration. -- **\n" );
#main.c:	gen_list_iterate( lcharptr_c,	(rec_iterate)my_custom_charptr_iterator_2 );
#main.c:
#main.c:
#main.c:	printf( "** -- Calling gen_list_iterate_reverse to iterate a charptr list -- **
#main.c:** -- NOTE: this iterator (my_custom_charptr_iterator_1) will iterate the whole list. -- **\n" );
#main.c:	gen_list_iterate_reverse( lcharptr,	(rec_iterate)my_custom_charptr_iterator_1 );
#main.c:
#main.c:	printf( "** -- Calling gen_list_iterate_reverse to iterate a copied charptr list -- **
#main.c:** -- NOTE: this iterator (my_custom_charptr_iterator_2) will return after the first iteration. -- **\n" );
#main.c:	gen_list_iterate_reverse( lcharptr_c,	(rec_iterate)my_custom_charptr_iterator_2 );
#main.c:
#main.c:
#main.c:	printf( "** -- Calling gen_list_equal to see if 2 different typed lists are equal  NOTE: they should not be. -- **\n" );
#main.c:	printf( "Are these 2 equal? => %d\n\n", gen_list_equal( lcharptr, llong ) );
#main.c:
#main.c:
#main.c:	printf( "** -- Calling gen_list_remove_all to erase a no-owner list -- **\n" );
#main.c:	no_own_list = gen_list_remove_all( no_own_list );
#main.c:
#main.c:	printf( "** -- Calling gen_list_destruct to destruct a no-owner list -- **\n" );
#main.c:	no_own_list = gen_list_destruct( no_own_list );
#main.c:
#main.c:
#main.c:	printf( "** -- Calling gen_list_remove to remove the first record of a copied charptr list -- **\n" );
#main.c:	derived_rec_charptr_destruct( (struct derived_rec_charptr *)gen_list_remove( lcharptr_c, gen_list_get_first( lcharptr_c ) ) );
#main.c:
#main.c:	printf( "** -- Calling gen_list_remove to remove the last record of a copied long list -- **\n" );
#main.c:	derived_rec_long_destruct( (struct derived_rec_long *)gen_list_remove( llong_c, gen_list_get_last( llong_c ) ) );
#main.c:
#main.c:	printf( "** -- Calling gen_list_remove to remove the middle record of an original charptr list -- **\n" );
#main.c:	derived_rec_charptr_destruct( (struct derived_rec_charptr *)gen_list_remove( lcharptr, f_rec ) );
#main.c:
#main.c:	printf( "** -- Calling gen_list_remove to remove the all 3 records of a copied char list (first to last) -- **\n" );
#main.c:	derived_rec_char_destruct( (struct derived_rec_char *)gen_list_remove( lchar_c, gen_list_get_first( lchar_c ) ) );
#main.c:	derived_rec_char_destruct( (struct derived_rec_char *)gen_list_remove( lchar_c, gen_list_get_first( lchar_c ) ) );
#main.c:	derived_rec_char_destruct( (struct derived_rec_char *)gen_list_remove( lchar_c, gen_list_get_first( lchar_c ) ) );
#main.c:
#main.c:	printf( "** -- Calling gen_list_remove to remove the all 3 records of the original char list (last to first) -- **\n" );
#main.c:	derived_rec_char_destruct( (struct derived_rec_char *)gen_list_remove( lchar, gen_list_get_last( lchar ) ) );
#main.c:	derived_rec_char_destruct( (struct derived_rec_char *)gen_list_remove( lchar, gen_list_get_last( lchar ) ) );
#main.c:	derived_rec_char_destruct( (struct derived_rec_char *)gen_list_remove( lchar, gen_list_get_last( lchar ) ) );
#main.c:
#main.c:
#main.c:	printf( "** -- Calling gen_list_destruct to destruct the original charptr list (less the middle record) -- **\n" );
#main.c:	lcharptr = gen_list_destruct( lcharptr );
#main.c:	printf( "** -- Calling gen_list_destruct to destruct the copied charptr list (less the first record) -- **\n" );
#main.c:	lcharptr_c = gen_list_destruct( lcharptr_c );
#main.c:
#main.c:	printf( "** -- Calling gen_list_destruct to destruct the original long list -- **\n" );
#main.c:	llong = gen_list_destruct( llong );
#main.c:	printf( "** -- Calling gen_list_destruct to destruct the copied long list (less the last record) -- **\n" );
#main.c:	llong_c = gen_list_destruct( llong_c );
#main.c:
#main.c:	printf( "** -- Calling gen_list_destruct to destruct the original double list -- **\n" );
#main.c:	ldouble = gen_list_destruct( ldouble );
#main.c:	printf( "** -- Calling gen_list_destruct to destruct the copied double list -- **\n" );
#main.c:	ldouble_c = gen_list_destruct( ldouble_c );
#main.c:
#main.c:	printf( "** -- Calling gen_list_destruct to destruct the original char list (less all records last to first) -- **\n" );
#main.c:	lchar = gen_list_destruct( lchar );
#main.c:	printf( "** -- Calling gen_list_destruct to destruct the copied char list (less all records first to last) -- **\n" );
#main.c:	lchar_c = gen_list_destruct( lchar_c );
#main.c:
#main.c:
#main.c:	/* Force another memory leak to test the strdup/malloc/free police. */
#main.c:	/* NOTE: this should trigger the alphanum check of allocated memory. */
#main.c:	STRDUP( "Force another memory leak to test the strdup/malloc/free police." );
#main.c:
#main.c:	/* Force a memory leak in a list and also in 2 nodes. */
#main.c:	/* NOTE: this should NOT trigger the alphanum check of allocated memory. */
#main.c:	gen_list_construct();
#main.c:
#main.c:	/* NOTE: this should trigger 1 NON check (for the node)	*/
#main.c:	/* followed by 1 alphanum check for the charptr string.	*/
#main.c:	derived_rec_charptr_construct( "Leaky charptr node." );
#main.c:
#main.c:	/* NOTE: this should NOT trigger the alphanum check of allocated memory. */
#main.c:	derived_rec_char_construct( 'Z' );
#main.c:
#main.c:
#main.c:	printf( "\ndone" );
#main.c:	return( 0 );
#main.c:
#main.c:}
#main.c:
#main.c:
#main.c:/*
#main.c:  Local Variables:
#main.c:  tab-width: 3
#main.c:  end:
#main.c:*/


#malloc_police.c:const char malloc_police_rcs[] = "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $";
#malloc_police.c:/*********************************************************************
#malloc_police.c: *
#malloc_police.c: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#malloc_police.c: *
#malloc_police.c: * Purpose     :  This module uses the rec_malloc_police to build a
#malloc_police.c: *						list of allocated and freed memory.  When the
#malloc_police.c: *						program exits, we will print a list of all memory
#malloc_police.c: *						that was allocated, but never freed.  This could
#malloc_police.c: *						be most helpful to developers and debugers.
#malloc_police.c: *
#malloc_police.c: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#malloc_police.c: *                Privoxy team. http://www.privoxy.org/
#malloc_police.c: *
#malloc_police.c: *                This program is free software; you can redistribute it
#malloc_police.c: *                and/or modify it under the terms of the GNU General
#malloc_police.c: *                Public License as published by the Free Software
#malloc_police.c: *                Foundation; either version 2 of the License, or (at
#malloc_police.c: *                your option) any later version.
#malloc_police.c: *
#malloc_police.c: *                This program is distributed in the hope that it will
#malloc_police.c: *                be useful, but WITHOUT ANY WARRANTY; without even the
#malloc_police.c: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#malloc_police.c: *                PARTICULAR PURPOSE.  See the GNU General Public
#malloc_police.c: *                License for more details.
#malloc_police.c: *
#malloc_police.c: *                The GNU General Public License should be included with
#malloc_police.c: *                this file.  If not, you can view it at
#malloc_police.c: *                http://www.gnu.org/copyleft/gpl.html
#malloc_police.c: *                or write to the Free Software Foundation, Inc., 59
#malloc_police.c: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#malloc_police.c: *
#malloc_police.c: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#malloc_police.c: *						and edit IJB, correctly.
#malloc_police.c: *
#malloc_police.c: * Revisions   :
#malloc_police.c: *    $Log: contrib.sh,v $
#malloc_police.c: *    Revision 1.3  2002/03/26 22:29:54  swa
#malloc_police.c: *    we have a new homepage!
#malloc_police.c: *
#malloc_police.c: *    Revision 1.2  2002/03/24 13:25:43  swa
#malloc_police.c: *    name change related issues
#malloc_police.c: *
#malloc_police.c: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#malloc_police.c: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#malloc_police.c: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#malloc_police.c: *    contrib directory and I cannot upload a tarball ... it gets
#malloc_police.c: *    corrupted).  This script will expand all files needed to create the
#malloc_police.c: *    linked list modules and an example program.  Please see the README.
#malloc_police.c: *    Feed back is welcomed.  Enjoy.
#malloc_police.c: *
#malloc_police.c: *
#malloc_police.c: *********************************************************************/
#malloc_police.c:
#malloc_police.c:
#malloc_police.c:#include <ctype.h>
#malloc_police.c:#include <malloc.h>
#malloc_police.c:#include <stdio.h>
#malloc_police.c:#include <stdlib.h>
#malloc_police.c:#include <string.h>
#malloc_police.c:
#malloc_police.c:#include "gen_list.h"
#malloc_police.c:#include "malloc_police.h"
#malloc_police.c:#include "rec_malloc_police.h"
#malloc_police.c:
#malloc_police.c:const char malloc_police_h_rcs[] = MALLOC_POLICE_H_VERSION;
#malloc_police.c:
#malloc_police.c:
#malloc_police.c:/* When we need to allocate malloc_police records, we */
#malloc_police.c:/* will do recursion infinately.  This variable will */
#malloc_police.c:/* stop the STRDUP, MALLOC, and FREE replacements from */
#malloc_police.c:/* trying to log these allocations. */
#malloc_police.c:
#malloc_police.c:char recursion_protect = 0;
#malloc_police.c:
#malloc_police.c:struct gen_list *get_police_list();
#malloc_police.c:static char called_once = 0;
#malloc_police.c:static const char *leak_report_header = "%s****************************** MEMORY LEAK REPORT %s ******************************%s";
#malloc_police.c:
#malloc_police.c:
#malloc_police.c:/*********************************************************************
#malloc_police.c: *
#malloc_police.c: * Function    :  police_list_release_iterator
#malloc_police.c: *
#malloc_police.c: * Description :  Iterator for the police list.  We will report
#malloc_police.c: *						"leaked" memory for the developer to correct.
#malloc_police.c: *
#malloc_police.c: * Parameters  :
#malloc_police.c: *          1  :  The record.
#malloc_police.c: *
#malloc_police.c: * Returns     :  NULL
#malloc_police.c: *
#malloc_police.c: *********************************************************************/
#malloc_police.c:struct derived_rec_malloc_police *police_list_release_iterator( struct derived_rec_malloc_police *this_rec )
#malloc_police.c:{
#malloc_police.c:	if ( ! called_once )
#malloc_police.c:	{
#malloc_police.c:		called_once = 1;
#malloc_police.c:		fprintf( stderr, leak_report_header, "\n\n", "BEGIN", "\n\n" );
#malloc_police.c:	}
#malloc_police.c:
#malloc_police.c:	fprintf( stderr, "\
#malloc_police.c:\t\tmalloc_police leaked memory iterator this_rec\t\t\t\t\t= %p
#malloc_police.c:\t\tmalloc_police leaked memory iterator this_rec->alloced_addr\t= %p
#malloc_police.c:\t\tmalloc_police leaked memory iterator this_rec->source_where\t= %s
#malloc_police.c:\t\tmalloc_police leaked memory iterator this_rec->sz\t\t\t\t= %ld\n",
#malloc_police.c:				(const void *)this_rec,
#malloc_police.c:				this_rec->alloced_addr,
#malloc_police.c:				this_rec->source_where,
#malloc_police.c:				this_rec->sz
#malloc_police.c:	);
#malloc_police.c:
#malloc_police.c:	if (
#malloc_police.c:		this_rec->sz >= 3 &&
#malloc_police.c:		isalnum( ((const char *)this_rec->alloced_addr)[ 0 ] ) &&
#malloc_police.c:		isalnum( ((const char *)this_rec->alloced_addr)[ 1 ] ) &&
#malloc_police.c:		isalnum( ((const char *)this_rec->alloced_addr)[ 2 ] ) )
#malloc_police.c:	{
#malloc_police.c:		/* If the memory starts with 3 alpha-numerics,
#malloc_police.c:		 * assume a string was allocated.  This might be
#malloc_police.c:		 * a little dangerous for production code, but I
#malloc_police.c:		 * will take the risk for development payoff
#malloc_police.c:		 * (at least for now ...) */
#malloc_police.c:			
#malloc_police.c:		fprintf( stderr, "\
#malloc_police.c:\t\tAlpha numeric found, assuming a string was allocated\t\t\t= %s\n",
#malloc_police.c:					this_rec->alloced_addr
#malloc_police.c:		);
#malloc_police.c:	}
#malloc_police.c:
#malloc_police.c:	fprintf( stderr, "\n" );
#malloc_police.c:
#malloc_police.c:	return( NULL );
#malloc_police.c:
#malloc_police.c:}
#malloc_police.c:
#malloc_police.c:
#malloc_police.c:/*********************************************************************
#malloc_police.c: *
#malloc_police.c: * Function    :  release_police_list
#malloc_police.c: *
#malloc_police.c: * Description :  Iterates the police_list and reports "leaked" memory.
#malloc_police.c: *						It will then free the list itself.
#malloc_police.c: *
#malloc_police.c: * Parameters  :	None
#malloc_police.c: *
#malloc_police.c: * Returns     :  None.
#malloc_police.c: *
#malloc_police.c: *********************************************************************/
#malloc_police.c:void release_police_list()
#malloc_police.c:{
#malloc_police.c:	struct gen_list *police_list = get_police_list();
#malloc_police.c:	fflush( stdout );
#malloc_police.c:	gen_list_iterate( police_list, (rec_iterate)police_list_release_iterator );
#malloc_police.c:
#malloc_police.c:	if ( called_once )
#malloc_police.c:	{
#malloc_police.c:		called_once = 0;
#malloc_police.c:		fprintf( stderr, leak_report_header, "", "END", "\n" );
#malloc_police.c:	}
#malloc_police.c:
#malloc_police.c:}
#malloc_police.c:
#malloc_police.c:
#malloc_police.c:/*********************************************************************
#malloc_police.c: *
#malloc_police.c: * Function    :  get_police_list
#malloc_police.c: *
#malloc_police.c: * Description :  Get the police malloc/strdup/free list.  Generate
#malloc_police.c: *						a new one if it does not yet exist.
#malloc_police.c: *
#malloc_police.c: * Parameters  :	None
#malloc_police.c: *
#malloc_police.c: * Returns     :  The police list.
#malloc_police.c: *
#malloc_police.c: *********************************************************************/
#malloc_police.c:struct gen_list *get_police_list()
#malloc_police.c:{
#malloc_police.c:	static struct gen_list *police_list = NULL;
#malloc_police.c:
#malloc_police.c:	if ( NULL == police_list )
#malloc_police.c:	{
#malloc_police.c:		recursion_protect ++;
#malloc_police.c:		list_is_quiet ++;
#malloc_police.c:
#malloc_police.c:		police_list = gen_list_construct();
#malloc_police.c:		atexit( release_police_list );
#malloc_police.c:
#malloc_police.c:		recursion_protect --;
#malloc_police.c:		list_is_quiet --;
#malloc_police.c:	}
#malloc_police.c:
#malloc_police.c:	return( police_list );
#malloc_police.c:
#malloc_police.c:}
#malloc_police.c:
#malloc_police.c:
#malloc_police.c:/*********************************************************************
#malloc_police.c: *
#malloc_police.c: * Function    :  police_strdup
#malloc_police.c: *
#malloc_police.c: * Description :  Save the results of the strdup into our linked list.
#malloc_police.c: *						This will be checked against latter free(s).
#malloc_police.c: *
#malloc_police.c: * Parameters  :
#malloc_police.c: *          1  :  The string to be duplicated.
#malloc_police.c: *          2  :  Filename where the strdup occured.
#malloc_police.c: *          3  :  Line# of where the strdup occured.
#malloc_police.c: *
#malloc_police.c: * Returns     :  Pointer to newly allocated memory.
#malloc_police.c: *
#malloc_police.c: * NOTE			:	This could replace the custom strdup for __MINGW32__
#malloc_police.c: *						systems.  We only need to copy the conditionally
#malloc_police.c: *						compiled code here and eliminate the strdup copy.
#malloc_police.c: *
#malloc_police.c: *********************************************************************/
#malloc_police.c:char *police_strdup( const char *s, const char *filename, long lineno )
#malloc_police.c:{
#malloc_police.c:	char *ret = strdup( s );
#malloc_police.c:
#malloc_police.c:	if ( 0 == recursion_protect )
#malloc_police.c:	{
#malloc_police.c:		char buffer[ 255 ];
#malloc_police.c:		sprintf( buffer, "strdup : %s @ %ld for %ld (%s)", filename, lineno, strlen( s ) + 1, s );
#malloc_police.c:
#malloc_police.c:		recursion_protect ++;
#malloc_police.c:		list_is_quiet ++;
#malloc_police.c:
#malloc_police.c:		gen_list_insert(
#malloc_police.c:			get_police_list(),
#malloc_police.c:			(struct gen_list_rec *)derived_rec_malloc_police_construct( ret, buffer, strlen( s ) + 1 )
#malloc_police.c:		);
#malloc_police.c:
#malloc_police.c:		recursion_protect --;
#malloc_police.c:		list_is_quiet --;
#malloc_police.c:	}
#malloc_police.c:
#malloc_police.c:	return( ret );
#malloc_police.c:
#malloc_police.c:}
#malloc_police.c:
#malloc_police.c:
#malloc_police.c:/*********************************************************************
#malloc_police.c: *
#malloc_police.c: * Function    :  police_malloc
#malloc_police.c: *
#malloc_police.c: * Description :  Save the results of the malloc into our linked list.
#malloc_police.c: *						This will be checked against latter free(s).
#malloc_police.c: *
#malloc_police.c: * Parameters  :
#malloc_police.c: *          1  :  The size of the memory to be malloc(ed).
#malloc_police.c: *          2  :  Filename where the malloc occured.
#malloc_police.c: *          3  :  Line# of where the malloc occured.
#malloc_police.c: *
#malloc_police.c: * Returns     :  0 => NOT EQUAL, anything else is EQUAL.
#malloc_police.c: *
#malloc_police.c: *********************************************************************/
#malloc_police.c:void *police_malloc( size_t sz, const char *filename, long lineno )
#malloc_police.c:{
#malloc_police.c:	void *ret = malloc( sz );
#malloc_police.c:
#malloc_police.c:	if ( 0 == recursion_protect )
#malloc_police.c:	{
#malloc_police.c:		char buffer[ 255 ];
#malloc_police.c:		sprintf( buffer, "malloc : %s @ %ld for %ld", filename, lineno, sz );
#malloc_police.c:
#malloc_police.c:		recursion_protect ++;
#malloc_police.c:		list_is_quiet ++;
#malloc_police.c:
#malloc_police.c:		gen_list_insert(
#malloc_police.c:			get_police_list(),
#malloc_police.c:			(struct gen_list_rec *)derived_rec_malloc_police_construct( ret, buffer, sz )
#malloc_police.c:		);
#malloc_police.c:
#malloc_police.c:		recursion_protect --;
#malloc_police.c:		list_is_quiet --;
#malloc_police.c:	}
#malloc_police.c:
#malloc_police.c:	return( ret );
#malloc_police.c:
#malloc_police.c:}
#malloc_police.c:
#malloc_police.c:
#malloc_police.c:/*********************************************************************
#malloc_police.c: *
#malloc_police.c: * Function    :  police_free
#malloc_police.c: *
#malloc_police.c: * Description :  Frees the memory allocation and removes the address
#malloc_police.c: *						from the linked list.  Anything left in this list
#malloc_police.c: *						is "leaked" memory.
#malloc_police.c: *
#malloc_police.c: * Parameters  :
#malloc_police.c: *          1  :  Pointer to the memory to be freed.
#malloc_police.c: *          2  :  Filename where the free occured.
#malloc_police.c: *          3  :  Line# of where the free occured.
#malloc_police.c: *
#malloc_police.c: * Returns     :  0 => NOT EQUAL, anything else is EQUAL.
#malloc_police.c: *
#malloc_police.c: *********************************************************************/
#malloc_police.c:void police_free( void *ptr, const char *filename, long lineno )
#malloc_police.c:{
#malloc_police.c:	if ( 0 == recursion_protect )
#malloc_police.c:	{
#malloc_police.c:		struct derived_rec_malloc_police *rec;
#malloc_police.c:		struct gen_list_rec *fRec;
#malloc_police.c:		struct gen_list *l = get_police_list();
#malloc_police.c:
#malloc_police.c:		recursion_protect ++;
#malloc_police.c:		list_is_quiet ++;
#malloc_police.c:
#malloc_police.c:		rec = derived_rec_malloc_police_construct( ptr, "FREEING POLICE RECORD.", 23 );
#malloc_police.c:		fRec = gen_list_find( l, (struct gen_list_rec *)rec );
#malloc_police.c:		derived_rec_malloc_police_destruct( rec );
#malloc_police.c:
#malloc_police.c:		if ( NULL == fRec )
#malloc_police.c:		{
#malloc_police.c:			fprintf( stderr, "\nERROR: free failed to find corresponding strdup/malloc : %s @ %ld\n", filename, lineno );
#malloc_police.c:		}
#malloc_police.c:		else
#malloc_police.c:		{
#malloc_police.c:			derived_rec_malloc_police_destruct( (struct derived_rec_malloc_police *)gen_list_remove( l, fRec ) );
#malloc_police.c:		}
#malloc_police.c:
#malloc_police.c:		recursion_protect --;
#malloc_police.c:		list_is_quiet --;
#malloc_police.c:	}
#malloc_police.c:
#malloc_police.c:	free( ptr );
#malloc_police.c:
#malloc_police.c:}


#malloc_police.h:#ifndef MALLOC_POLICE_H_INCLUDED
#malloc_police.h:#define MALLOC_POLICE_H_INCLUDED
#malloc_police.h:#define MALLOC_POLICE_H_VERSION "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $"
#malloc_police.h:/*********************************************************************
#malloc_police.h: *
#malloc_police.h: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#malloc_police.h: *
#malloc_police.h: * Purpose     :  This module uses the rec_malloc_police to build a
#malloc_police.h: *						list of allocated and freed memory.  When the
#malloc_police.h: *						program exits, we will print a list of all memory
#malloc_police.h: *						that was allocated, but never freed.  This could
#malloc_police.h: *						be most helpful to developers and debugers.
#malloc_police.h: *
#malloc_police.h: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#malloc_police.h: *                Privoxy team. http://www.privoxy.org/
#malloc_police.h: *
#malloc_police.h: *                This program is free software; you can redistribute it
#malloc_police.h: *                and/or modify it under the terms of the GNU General
#malloc_police.h: *                Public License as published by the Free Software
#malloc_police.h: *                Foundation; either version 2 of the License, or (at
#malloc_police.h: *                your option) any later version.
#malloc_police.h: *
#malloc_police.h: *                This program is distributed in the hope that it will
#malloc_police.h: *                be useful, but WITHOUT ANY WARRANTY; without even the
#malloc_police.h: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#malloc_police.h: *                PARTICULAR PURPOSE.  See the GNU General Public
#malloc_police.h: *                License for more details.
#malloc_police.h: *
#malloc_police.h: *                The GNU General Public License should be included with
#malloc_police.h: *                this file.  If not, you can view it at
#malloc_police.h: *                http://www.gnu.org/copyleft/gpl.html
#malloc_police.h: *                or write to the Free Software Foundation, Inc., 59
#malloc_police.h: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#malloc_police.h: *
#malloc_police.h: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#malloc_police.h: *						and edit IJB, correctly.
#malloc_police.h: *
#malloc_police.h: * Revisions   :
#malloc_police.h: *    $Log: contrib.sh,v $
#malloc_police.h: *    Revision 1.3  2002/03/26 22:29:54  swa
#malloc_police.h: *    we have a new homepage!
#malloc_police.h: *
#malloc_police.h: *    Revision 1.2  2002/03/24 13:25:43  swa
#malloc_police.h: *    name change related issues
#malloc_police.h: *
#malloc_police.h: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#malloc_police.h: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#malloc_police.h: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#malloc_police.h: *    contrib directory and I cannot upload a tarball ... it gets
#malloc_police.h: *    corrupted).  This script will expand all files needed to create the
#malloc_police.h: *    linked list modules and an example program.  Please see the README.
#malloc_police.h: *    Feed back is welcomed.  Enjoy.
#malloc_police.h: *
#malloc_police.h: *
#malloc_police.h: *********************************************************************/
#malloc_police.h:
#malloc_police.h:
#malloc_police.h:#ifdef __cplusplus
#malloc_police.h:extern "C" {
#malloc_police.h:#endif
#malloc_police.h:
#malloc_police.h:
#malloc_police.h:char *police_strdup( const char *s, const char *filename, long lineno );
#malloc_police.h:void *police_malloc( size_t sz, const char *filename, long lineno );
#malloc_police.h:void police_free( void *ptr, const char *filename, long lineno );
#malloc_police.h:
#malloc_police.h:#define STRDUP(s)			police_strdup( s, __FILE__, __LINE__ )
#malloc_police.h:#define MALLOC(sz)		police_malloc( sz, __FILE__, __LINE__ )
#malloc_police.h:#define FREE(ptr)			police_free( ptr, __FILE__, __LINE__ )
#malloc_police.h:
#malloc_police.h:
#malloc_police.h:#ifdef __cplusplus
#malloc_police.h:} /* extern "C" */
#malloc_police.h:#endif
#malloc_police.h:
#malloc_police.h:#endif /* ndef MALLOC_POLICE_H_INCLUDED */
#malloc_police.h:
#malloc_police.h:/*
#malloc_police.h:  Local Variables:
#malloc_police.h:  tab-width: 3
#malloc_police.h:  end:
#malloc_police.h:*/


#rec_char.c:const char rec_char_rcs[] = "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $";
#rec_char.c:/*********************************************************************
#rec_char.c: *
#rec_char.c: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#rec_char.c: *
#rec_char.c: * Purpose     :  A "derived class" of gen_list_rec.
#rec_char.c: *
#rec_char.c: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#rec_char.c: *                Privoxy team. http://www.privoxy.org/
#rec_char.c: *
#rec_char.c: *                This program is free software; you can redistribute it
#rec_char.c: *                and/or modify it under the terms of the GNU General
#rec_char.c: *                Public License as published by the Free Software
#rec_char.c: *                Foundation; either version 2 of the License, or (at
#rec_char.c: *                your option) any later version.
#rec_char.c: *
#rec_char.c: *                This program is distributed in the hope that it will
#rec_char.c: *                be useful, but WITHOUT ANY WARRANTY; without even the
#rec_char.c: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#rec_char.c: *                PARTICULAR PURPOSE.  See the GNU General Public
#rec_char.c: *                License for more details.
#rec_char.c: *
#rec_char.c: *                The GNU General Public License should be included with
#rec_char.c: *                this file.  If not, you can view it at
#rec_char.c: *                http://www.gnu.org/copyleft/gpl.html
#rec_char.c: *                or write to the Free Software Foundation, Inc., 59
#rec_char.c: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#rec_char.c: *
#rec_char.c: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#rec_char.c: *						and edit IJB, correctly.
#rec_char.c: *
#rec_char.c: * Revisions   :
#rec_char.c: *    $Log: contrib.sh,v $
#rec_char.c: *    Revision 1.3  2002/03/26 22:29:54  swa
#rec_char.c: *    we have a new homepage!
#rec_char.c: *
#rec_char.c: *    Revision 1.2  2002/03/24 13:25:43  swa
#rec_char.c: *    name change related issues
#rec_char.c: *
#rec_char.c: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#rec_char.c: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#rec_char.c: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#rec_char.c: *    contrib directory and I cannot upload a tarball ... it gets
#rec_char.c: *    corrupted).  This script will expand all files needed to create the
#rec_char.c: *    linked list modules and an example program.  Please see the README.
#rec_char.c: *    Feed back is welcomed.  Enjoy.
#rec_char.c: *
#rec_char.c: *
#rec_char.c: *********************************************************************/
#rec_char.c:
#rec_char.c:
#rec_char.c:#include <malloc.h>
#rec_char.c:#include <stdio.h>
#rec_char.c:#include <stdlib.h>
#rec_char.c:#include <string.h>
#rec_char.c:
#rec_char.c:#include "gen_list.h"
#rec_char.c:#include "rec_char.h"
#rec_char.c:
#rec_char.c:const char rec_char_h_rcs[] = REC_CHAR_H_VERSION;
#rec_char.c:
#rec_char.c:
#rec_char.c:static const rec_method rec_char_vtable[] =
#rec_char.c:{
#rec_char.c:	(rec_method)derived_rec_char_copy_construct,
#rec_char.c:	(rec_method)derived_rec_char_destruct,
#rec_char.c:	(rec_method)derived_rec_char_stream,
#rec_char.c:	(rec_method)derived_rec_char_equal
#rec_char.c:};
#rec_char.c:
#rec_char.c:
#rec_char.c:/*********************************************************************
#rec_char.c: *
#rec_char.c: * Function    :  derived_rec_char_construct
#rec_char.c: *
#rec_char.c: * Description :  A simple derived record class that contains 1 string.
#rec_char.c: *
#rec_char.c: * Parameters  :
#rec_char.c: *          1  :  The record
#rec_char.c: *          2  :  The string to contain.
#rec_char.c: *
#rec_char.c: * Returns     :  A pointer to the constructed record.
#rec_char.c: *
#rec_char.c: *********************************************************************/
#rec_char.c:struct derived_rec_char *derived_rec_char_construct( const char _contents )
#rec_char.c:{
#rec_char.c:	struct derived_rec_char *this_rec = (struct derived_rec_char *)gen_list_rec_construct(
#rec_char.c:		ISA_CHAR,
#rec_char.c:		sizeof( struct derived_rec_char ),
#rec_char.c:		rec_char_vtable
#rec_char.c:	);
#rec_char.c:
#rec_char.c:	this_rec->contents = _contents;
#rec_char.c:
#rec_char.c:	LIST_SHOW( printf( "\
#rec_char.c:\t\tchar construct new rec\t\t\t\t= %p
#rec_char.c:\t\tchar construct new rec contents\t= %c\n\n", (const void *)this_rec, this_rec->contents ) );
#rec_char.c:
#rec_char.c:	return( this_rec );
#rec_char.c:
#rec_char.c:}
#rec_char.c:
#rec_char.c:
#rec_char.c:/*********************************************************************
#rec_char.c: *
#rec_char.c: * Function    :  derived_rec_char_copy_construct
#rec_char.c: *
#rec_char.c: * Description :  Copies one char record to another.
#rec_char.c: *
#rec_char.c: * Parameters  :
#rec_char.c: *          1  :  Existing record.
#rec_char.c: *				2	:  Copy record.
#rec_char.c: *
#rec_char.c: * Returns     :  The newly constructed copy record.
#rec_char.c: *
#rec_char.c: *********************************************************************/
#rec_char.c:struct derived_rec_char *derived_rec_char_copy_construct( const struct derived_rec_char *this_rec )
#rec_char.c:{
#rec_char.c:	struct derived_rec_char *copy_rec = (struct derived_rec_char *)gen_list_rec_copy_construct( (struct gen_list_rec *)this_rec );
#rec_char.c:	copy_rec->contents = this_rec->contents;
#rec_char.c:
#rec_char.c:	LIST_SHOW( printf( "\
#rec_char.c:\t\tchar copy construct new gen rec\t\t\t\t= %p => %p
#rec_char.c:\t\tchar copy construct new gen rec contents\t= %c\n\n", (const void *)this_rec, (const void *)copy_rec, copy_rec->contents ) );
#rec_char.c:
#rec_char.c:	return( copy_rec );
#rec_char.c:
#rec_char.c:}
#rec_char.c:
#rec_char.c:
#rec_char.c:/*********************************************************************
#rec_char.c: *
#rec_char.c: * Function    :  derived_rec_char_destruct	
#rec_char.c: *
#rec_char.c: * Description :  Destruct the char record.
#rec_char.c: *
#rec_char.c: * Parameters  :
#rec_char.c: *          1  :  The record.
#rec_char.c: *
#rec_char.c: * Returns     :  NULL
#rec_char.c: *
#rec_char.c: *********************************************************************/
#rec_char.c:struct derived_rec_char *derived_rec_char_destruct( struct derived_rec_char *this_rec )
#rec_char.c:{
#rec_char.c:	LIST_SHOW( printf( "\
#rec_char.c:\t\tchar destruct this_rec\t\t\t\t= %p
#rec_char.c:\t\tchar destruct this_rec->contents\t= %c\n\n", (const void *)this_rec, this_rec->contents ) );
#rec_char.c:
#rec_char.c:	this_rec->contents = '!';
#rec_char.c:	return( (struct derived_rec_char *)gen_list_rec_destruct( (struct gen_list_rec *)this_rec ) );
#rec_char.c:
#rec_char.c:}
#rec_char.c:
#rec_char.c:
#rec_char.c:/*********************************************************************
#rec_char.c: *
#rec_char.c: * Function    :  derived_rec_char_stream
#rec_char.c: *
#rec_char.c: * Description :  Displays all char attributes on the STDOUT stream.
#rec_char.c: *
#rec_char.c: * Parameters  :
#rec_char.c: *          1  :  The record.
#rec_char.c: *
#rec_char.c: * Returns     :  The record.
#rec_char.c: *
#rec_char.c: *********************************************************************/
#rec_char.c:const struct derived_rec_char *derived_rec_char_stream( const struct derived_rec_char *this_rec )
#rec_char.c:{
#rec_char.c:	this_rec = (struct derived_rec_char *)gen_list_rec_stream(
#rec_char.c:		(struct gen_list_rec *)this_rec
#rec_char.c:	);
#rec_char.c:	LIST_SHOW( printf( "\
#rec_char.c:\t\tchar stream this_rec\t\t\t\t\t= %p
#rec_char.c:\t\tchar stream this_rec->contents\t= %c\n\n", (const void *)this_rec, this_rec->contents ) );
#rec_char.c:
#rec_char.c:	return( this_rec );
#rec_char.c:
#rec_char.c:}
#rec_char.c:
#rec_char.c:
#rec_char.c:/*********************************************************************
#rec_char.c: *
#rec_char.c: * Function    :  derived_rec_char_equal
#rec_char.c: *
#rec_char.c: * Description :  Compares two char records to see if they are equal.
#rec_char.c: *
#rec_char.c: * Parameters  :
#rec_char.c: *          1  :  A record.
#rec_char.c: *          2  :  Another record.
#rec_char.c: *
#rec_char.c: * Returns     :  0 => NOT EQUAL, anything else is EQUAL.
#rec_char.c: *
#rec_char.c: *********************************************************************/
#rec_char.c:int derived_rec_char_equal( const struct derived_rec_char *this_rec, const struct derived_rec_char *eq_rec )
#rec_char.c:{
#rec_char.c:	if ( ! gen_list_rec_equal( (const struct gen_list_rec *)this_rec, (struct gen_list_rec *)eq_rec ) )
#rec_char.c:	{
#rec_char.c:		return( 0 );
#rec_char.c:	}
#rec_char.c:	return( this_rec->contents == eq_rec->contents );
#rec_char.c:
#rec_char.c:}


#rec_char.h:#ifndef REC_CHAR_H_INCLUDED
#rec_char.h:#define REC_CHAR_H_INCLUDED
#rec_char.h:#define REC_CHAR_H_VERSION "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $"
#rec_char.h:/*********************************************************************
#rec_char.h: *
#rec_char.h: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#rec_char.h: *
#rec_char.h: * Purpose     :  A "derived class" of gen_list_rec.
#rec_char.h: *
#rec_char.h: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#rec_char.h: *                Privoxy team. http://www.privoxy.org/
#rec_char.h: *
#rec_char.h: *                This program is free software; you can redistribute it
#rec_char.h: *                and/or modify it under the terms of the GNU General
#rec_char.h: *                Public License as published by the Free Software
#rec_char.h: *                Foundation; either version 2 of the License, or (at
#rec_char.h: *                your option) any later version.
#rec_char.h: *
#rec_char.h: *                This program is distributed in the hope that it will
#rec_char.h: *                be useful, but WITHOUT ANY WARRANTY; without even the
#rec_char.h: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#rec_char.h: *                PARTICULAR PURPOSE.  See the GNU General Public
#rec_char.h: *                License for more details.
#rec_char.h: *
#rec_char.h: *                The GNU General Public License should be included with
#rec_char.h: *                this file.  If not, you can view it at
#rec_char.h: *                http://www.gnu.org/copyleft/gpl.html
#rec_char.h: *                or write to the Free Software Foundation, Inc., 59
#rec_char.h: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#rec_char.h: *
#rec_char.h: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#rec_char.h: *						and edit IJB, correctly.
#rec_char.h: *
#rec_char.h: * Revisions   :
#rec_char.h: *    $Log: contrib.sh,v $
#rec_char.h: *    Revision 1.3  2002/03/26 22:29:54  swa
#rec_char.h: *    we have a new homepage!
#rec_char.h: *
#rec_char.h: *    Revision 1.2  2002/03/24 13:25:43  swa
#rec_char.h: *    name change related issues
#rec_char.h: *
#rec_char.h: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#rec_char.h: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#rec_char.h: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#rec_char.h: *    contrib directory and I cannot upload a tarball ... it gets
#rec_char.h: *    corrupted).  This script will expand all files needed to create the
#rec_char.h: *    linked list modules and an example program.  Please see the README.
#rec_char.h: *    Feed back is welcomed.  Enjoy.
#rec_char.h: *
#rec_char.h: *
#rec_char.h: *********************************************************************/
#rec_char.h:
#rec_char.h:
#rec_char.h:#ifdef __cplusplus
#rec_char.h:extern "C" {
#rec_char.h:#endif
#rec_char.h:
#rec_char.h:
#rec_char.h:struct derived_rec_char
#rec_char.h:{
#rec_char.h:	/* private: */
#rec_char.h:	struct gen_list_rec parent_rec;
#rec_char.h:	char contents;
#rec_char.h:};
#rec_char.h:
#rec_char.h:/* public: */
#rec_char.h:extern struct derived_rec_char *	derived_rec_char_construct( const char _contents );
#rec_char.h:extern struct derived_rec_char *	derived_rec_char_copy_construct( const struct derived_rec_char *this_rec );
#rec_char.h:extern struct derived_rec_char *	derived_rec_char_destruct( struct derived_rec_char *this_rec );
#rec_char.h:extern const struct derived_rec_char *derived_rec_char_stream( const struct derived_rec_char *this_rec );
#rec_char.h:extern int								derived_rec_char_equal( const struct derived_rec_char *this_rec, const struct derived_rec_char *eq_rec );
#rec_char.h:
#rec_char.h:/* struct/class COMPLETE */
#rec_char.h:
#rec_char.h:
#rec_char.h:#ifdef __cplusplus
#rec_char.h:} /* extern "C" */
#rec_char.h:#endif
#rec_char.h:
#rec_char.h:#endif /* ndef REC_CHAR_H_INCLUDED */
#rec_char.h:
#rec_char.h:/*
#rec_char.h:  Local Variables:
#rec_char.h:  tab-width: 3
#rec_char.h:  end:
#rec_char.h:*/


#rec_charptr.c:const char rec_charptr_rcs[] = "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $";
#rec_charptr.c:/*********************************************************************
#rec_charptr.c: *
#rec_charptr.c: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#rec_charptr.c: *
#rec_charptr.c: * Purpose     :  A "derived class" of gen_list_rec.
#rec_charptr.c: *
#rec_charptr.c: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#rec_charptr.c: *                Privoxy team. http://www.privoxy.org/
#rec_charptr.c: *
#rec_charptr.c: *                This program is free software; you can redistribute it
#rec_charptr.c: *                and/or modify it under the terms of the GNU General
#rec_charptr.c: *                Public License as published by the Free Software
#rec_charptr.c: *                Foundation; either version 2 of the License, or (at
#rec_charptr.c: *                your option) any later version.
#rec_charptr.c: *
#rec_charptr.c: *                This program is distributed in the hope that it will
#rec_charptr.c: *                be useful, but WITHOUT ANY WARRANTY; without even the
#rec_charptr.c: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#rec_charptr.c: *                PARTICULAR PURPOSE.  See the GNU General Public
#rec_charptr.c: *                License for more details.
#rec_charptr.c: *
#rec_charptr.c: *                The GNU General Public License should be included with
#rec_charptr.c: *                this file.  If not, you can view it at
#rec_charptr.c: *                http://www.gnu.org/copyleft/gpl.html
#rec_charptr.c: *                or write to the Free Software Foundation, Inc., 59
#rec_charptr.c: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#rec_charptr.c: *
#rec_charptr.c: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#rec_charptr.c: *						and edit IJB, correctly.
#rec_charptr.c: *
#rec_charptr.c: * Revisions   :
#rec_charptr.c: *    $Log: contrib.sh,v $
#rec_charptr.c: *    Revision 1.3  2002/03/26 22:29:54  swa
#rec_charptr.c: *    we have a new homepage!
#rec_charptr.c: *
#rec_charptr.c: *    Revision 1.2  2002/03/24 13:25:43  swa
#rec_charptr.c: *    name change related issues
#rec_charptr.c: *
#rec_charptr.c: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#rec_charptr.c: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#rec_charptr.c: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#rec_charptr.c: *    contrib directory and I cannot upload a tarball ... it gets
#rec_charptr.c: *    corrupted).  This script will expand all files needed to create the
#rec_charptr.c: *    linked list modules and an example program.  Please see the README.
#rec_charptr.c: *    Feed back is welcomed.  Enjoy.
#rec_charptr.c: *
#rec_charptr.c: *
#rec_charptr.c: *********************************************************************/
#rec_charptr.c:
#rec_charptr.c:
#rec_charptr.c:#include <malloc.h>
#rec_charptr.c:#include <stdio.h>
#rec_charptr.c:#include <stdlib.h>
#rec_charptr.c:#include <string.h>
#rec_charptr.c:
#rec_charptr.c:#include "gen_list.h"
#rec_charptr.c:#include "rec_charptr.h"
#rec_charptr.c:#include "malloc_police.h"
#rec_charptr.c:
#rec_charptr.c:const char rec_charptr_h_rcs[] = REC_CHARPTR_H_VERSION;
#rec_charptr.c:
#rec_charptr.c:
#rec_charptr.c:static const rec_method rec_charptr_vtable[] =
#rec_charptr.c:{
#rec_charptr.c:	(rec_method)derived_rec_charptr_copy_construct,
#rec_charptr.c:	(rec_method)derived_rec_charptr_destruct,
#rec_charptr.c:	(rec_method)derived_rec_charptr_stream,
#rec_charptr.c:	(rec_method)derived_rec_charptr_equal
#rec_charptr.c:};
#rec_charptr.c:
#rec_charptr.c:
#rec_charptr.c:/*********************************************************************
#rec_charptr.c: *
#rec_charptr.c: * Function    :  derived_rec_charptr_construct
#rec_charptr.c: *
#rec_charptr.c: * Description :  A simple derived record class that contains 1 string.
#rec_charptr.c: *
#rec_charptr.c: * Parameters  :
#rec_charptr.c: *          1  :  The record
#rec_charptr.c: *          2  :  The string to contain.
#rec_charptr.c: *
#rec_charptr.c: * Returns     :  A pointer to the constructed record.
#rec_charptr.c: *
#rec_charptr.c: *********************************************************************/
#rec_charptr.c:struct derived_rec_charptr *derived_rec_charptr_construct( const char *_contents )
#rec_charptr.c:{
#rec_charptr.c:	struct derived_rec_charptr *this_rec = (struct derived_rec_charptr *)gen_list_rec_construct(
#rec_charptr.c:		ISA_CHARPTR,
#rec_charptr.c:		sizeof( struct derived_rec_charptr ),
#rec_charptr.c:		rec_charptr_vtable
#rec_charptr.c:	);
#rec_charptr.c:
#rec_charptr.c:	this_rec->contents = STRDUP( _contents );
#rec_charptr.c:
#rec_charptr.c:	LIST_SHOW( printf( "\
#rec_charptr.c:\t\tcharptr construct new rec\t\t\t\t= %p
#rec_charptr.c:\t\tcharptr construct new rec contents\t= %s\n\n", (const void *)this_rec, this_rec->contents ) );
#rec_charptr.c:
#rec_charptr.c:	return( this_rec );
#rec_charptr.c:
#rec_charptr.c:}
#rec_charptr.c:
#rec_charptr.c:
#rec_charptr.c:/*********************************************************************
#rec_charptr.c: *
#rec_charptr.c: * Function    :  derived_rec_charptr_copy_construct
#rec_charptr.c: *
#rec_charptr.c: * Description :  Copies one charptr record to another.
#rec_charptr.c: *
#rec_charptr.c: * Parameters  :
#rec_charptr.c: *          1  :  Existing record.
#rec_charptr.c: *				2	:  Copy record.
#rec_charptr.c: *
#rec_charptr.c: * Returns     :  The newly constructed copy record.
#rec_charptr.c: *
#rec_charptr.c: *********************************************************************/
#rec_charptr.c:struct derived_rec_charptr *derived_rec_charptr_copy_construct( const struct derived_rec_charptr *this_rec )
#rec_charptr.c:{
#rec_charptr.c:	int len;
#rec_charptr.c:	char *new_contents;
#rec_charptr.c:
#rec_charptr.c:	struct derived_rec_charptr *copy_rec = (struct derived_rec_charptr *)gen_list_rec_copy_construct( (struct gen_list_rec *)this_rec );
#rec_charptr.c:
#rec_charptr.c:	len = strlen( this_rec->contents );
#rec_charptr.c:	len += sizeof( "COPY " );
#rec_charptr.c:
#rec_charptr.c:	copy_rec->contents = (char *)MALLOC( len );
#rec_charptr.c:	strcpy( copy_rec->contents, "COPY " );
#rec_charptr.c:	strcat( copy_rec->contents, this_rec->contents );
#rec_charptr.c:
#rec_charptr.c:	LIST_SHOW( printf( "\
#rec_charptr.c:\t\tcharptr copy construct new gen rec\t\t\t\t= %p => %p
#rec_charptr.c:\t\tcharptr copy construct new gen rec contents\t= %s\n\n", (const void *)this_rec, (const void *)copy_rec, copy_rec->contents ) );
#rec_charptr.c:
#rec_charptr.c:
#rec_charptr.c:	return( copy_rec );
#rec_charptr.c:
#rec_charptr.c:}
#rec_charptr.c:
#rec_charptr.c:
#rec_charptr.c:/*********************************************************************
#rec_charptr.c: *
#rec_charptr.c: * Function    :  derived_rec_charptr_destruct	
#rec_charptr.c: *
#rec_charptr.c: * Description :  Destruct the charptr record.
#rec_charptr.c: *
#rec_charptr.c: * Parameters  :
#rec_charptr.c: *          1  :  The record.
#rec_charptr.c: *
#rec_charptr.c: * Returns     :  NULL
#rec_charptr.c: *
#rec_charptr.c: *********************************************************************/
#rec_charptr.c:struct derived_rec_charptr *derived_rec_charptr_destruct( struct derived_rec_charptr *this_rec )
#rec_charptr.c:{
#rec_charptr.c:	LIST_SHOW( printf( "\
#rec_charptr.c:\t\tcharptr destruct this_rec\t\t\t\t= %p
#rec_charptr.c:\t\tcharptr destruct this_rec->contents\t= %s\n\n", (const void *)this_rec, (const void *)this_rec->contents ) );
#rec_charptr.c:
#rec_charptr.c:	memset( this_rec->contents, '!', strlen( this_rec->contents ) );
#rec_charptr.c:	FREE( this_rec->contents );
#rec_charptr.c:	return( (struct derived_rec_charptr *)gen_list_rec_destruct( (struct gen_list_rec *)this_rec ) );
#rec_charptr.c:
#rec_charptr.c:}
#rec_charptr.c:
#rec_charptr.c:
#rec_charptr.c:/*********************************************************************
#rec_charptr.c: *
#rec_charptr.c: * Function    :  derived_rec_charptr_stream
#rec_charptr.c: *
#rec_charptr.c: * Description :  Displays all charptr attributes on the STDOUT stream.
#rec_charptr.c: *
#rec_charptr.c: * Parameters  :
#rec_charptr.c: *          1  :  The record.
#rec_charptr.c: *
#rec_charptr.c: * Returns     :  The record.
#rec_charptr.c: *
#rec_charptr.c: *********************************************************************/
#rec_charptr.c:const struct derived_rec_charptr *derived_rec_charptr_stream( const struct derived_rec_charptr *this_rec )
#rec_charptr.c:{
#rec_charptr.c:	this_rec = (struct derived_rec_charptr *)gen_list_rec_stream(
#rec_charptr.c:		(struct gen_list_rec *)this_rec
#rec_charptr.c:	);
#rec_charptr.c:	LIST_SHOW( printf( "\
#rec_charptr.c:\t\tcharptr stream this_rec\t\t\t\t\t= %p
#rec_charptr.c:\t\tcharptr stream this_rec->contents\t= %s\n\n", (const void *)this_rec, this_rec->contents ) );
#rec_charptr.c:
#rec_charptr.c:	return( this_rec );
#rec_charptr.c:
#rec_charptr.c:}
#rec_charptr.c:
#rec_charptr.c:
#rec_charptr.c:/*********************************************************************
#rec_charptr.c: *
#rec_charptr.c: * Function    :  derived_rec_charptr_equal
#rec_charptr.c: *
#rec_charptr.c: * Description :  Compares two charptr records to see if they are equal.
#rec_charptr.c: *
#rec_charptr.c: * Parameters  :
#rec_charptr.c: *          1  :  A record.
#rec_charptr.c: *          2  :  Another record.
#rec_charptr.c: *
#rec_charptr.c: * Returns     :  0 => NOT EQUAL, anything else is EQUAL.
#rec_charptr.c: *
#rec_charptr.c: *********************************************************************/
#rec_charptr.c:int derived_rec_charptr_equal( const struct derived_rec_charptr *this_rec, const struct derived_rec_charptr *eq_rec )
#rec_charptr.c:{
#rec_charptr.c:	if ( ! gen_list_rec_equal( (const struct gen_list_rec *)this_rec, (struct gen_list_rec *)eq_rec ) )
#rec_charptr.c:	{
#rec_charptr.c:		return( 0 );
#rec_charptr.c:	}
#rec_charptr.c:	return( 0 == strcmp( this_rec->contents, eq_rec->contents ) );
#rec_charptr.c:
#rec_charptr.c:}


#rec_charptr.h:#ifndef REC_CHARPTR_H_INCLUDED
#rec_charptr.h:#define REC_CHARPTR_H_INCLUDED
#rec_charptr.h:#define REC_CHARPTR_H_VERSION "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $"
#rec_charptr.h:/*********************************************************************
#rec_charptr.h: *
#rec_charptr.h: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#rec_charptr.h: *
#rec_charptr.h: * Purpose     :  A "derived class" of gen_list_rec.
#rec_charptr.h: *
#rec_charptr.h: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#rec_charptr.h: *                Privoxy team. http://www.privoxy.org/
#rec_charptr.h: *
#rec_charptr.h: *                This program is free software; you can redistribute it
#rec_charptr.h: *                and/or modify it under the terms of the GNU General
#rec_charptr.h: *                Public License as published by the Free Software
#rec_charptr.h: *                Foundation; either version 2 of the License, or (at
#rec_charptr.h: *                your option) any later version.
#rec_charptr.h: *
#rec_charptr.h: *                This program is distributed in the hope that it will
#rec_charptr.h: *                be useful, but WITHOUT ANY WARRANTY; without even the
#rec_charptr.h: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#rec_charptr.h: *                PARTICULAR PURPOSE.  See the GNU General Public
#rec_charptr.h: *                License for more details.
#rec_charptr.h: *
#rec_charptr.h: *                The GNU General Public License should be included with
#rec_charptr.h: *                this file.  If not, you can view it at
#rec_charptr.h: *                http://www.gnu.org/copyleft/gpl.html
#rec_charptr.h: *                or write to the Free Software Foundation, Inc., 59
#rec_charptr.h: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#rec_charptr.h: *
#rec_charptr.h: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#rec_charptr.h: *						and edit IJB, correctly.
#rec_charptr.h: *
#rec_charptr.h: * Revisions   :
#rec_charptr.h: *    $Log: contrib.sh,v $
#rec_charptr.h: *    Revision 1.3  2002/03/26 22:29:54  swa
#rec_charptr.h: *    we have a new homepage!
#rec_charptr.h: *
#rec_charptr.h: *    Revision 1.2  2002/03/24 13:25:43  swa
#rec_charptr.h: *    name change related issues
#rec_charptr.h: *
#rec_charptr.h: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#rec_charptr.h: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#rec_charptr.h: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#rec_charptr.h: *    contrib directory and I cannot upload a tarball ... it gets
#rec_charptr.h: *    corrupted).  This script will expand all files needed to create the
#rec_charptr.h: *    linked list modules and an example program.  Please see the README.
#rec_charptr.h: *    Feed back is welcomed.  Enjoy.
#rec_charptr.h: *
#rec_charptr.h: *
#rec_charptr.h: *********************************************************************/
#rec_charptr.h:
#rec_charptr.h:
#rec_charptr.h:#ifdef __cplusplus
#rec_charptr.h:extern "C" {
#rec_charptr.h:#endif
#rec_charptr.h:
#rec_charptr.h:
#rec_charptr.h:struct derived_rec_charptr
#rec_charptr.h:{
#rec_charptr.h:	/* private: */
#rec_charptr.h:	struct gen_list_rec parent_rec;
#rec_charptr.h:	char *contents;
#rec_charptr.h:};
#rec_charptr.h:
#rec_charptr.h:/* public: */
#rec_charptr.h:extern struct derived_rec_charptr *	derived_rec_charptr_construct( const char *_contents );
#rec_charptr.h:extern struct derived_rec_charptr *	derived_rec_charptr_copy_construct( const struct derived_rec_charptr *this_rec );
#rec_charptr.h:extern struct derived_rec_charptr *	derived_rec_charptr_destruct( struct derived_rec_charptr *this_rec );
#rec_charptr.h:extern const struct derived_rec_charptr *derived_rec_charptr_stream( const struct derived_rec_charptr *this_rec );
#rec_charptr.h:extern int									derived_rec_charptr_equal( const struct derived_rec_charptr *this_rec, const struct derived_rec_charptr *eq_rec );
#rec_charptr.h:
#rec_charptr.h:/* struct/class COMPLETE */
#rec_charptr.h:
#rec_charptr.h:
#rec_charptr.h:#ifdef __cplusplus
#rec_charptr.h:} /* extern "C" */
#rec_charptr.h:#endif
#rec_charptr.h:
#rec_charptr.h:#endif /* ndef REC_CHARPTR_H_INCLUDED */
#rec_charptr.h:
#rec_charptr.h:/*
#rec_charptr.h:  Local Variables:
#rec_charptr.h:  tab-width: 3
#rec_charptr.h:  end:
#rec_charptr.h:*/


#rec_double.c:const char rec_double_rcs[] = "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $";
#rec_double.c:/*********************************************************************
#rec_double.c: *
#rec_double.c: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#rec_double.c: *
#rec_double.c: * Purpose     :  A "derived class" of gen_list_rec.
#rec_double.c: *
#rec_double.c: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#rec_double.c: *                Privoxy team. http://www.privoxy.org/
#rec_double.c: *
#rec_double.c: *                This program is free software; you can redistribute it
#rec_double.c: *                and/or modify it under the terms of the GNU General
#rec_double.c: *                Public License as published by the Free Software
#rec_double.c: *                Foundation; either version 2 of the License, or (at
#rec_double.c: *                your option) any later version.
#rec_double.c: *
#rec_double.c: *                This program is distributed in the hope that it will
#rec_double.c: *                be useful, but WITHOUT ANY WARRANTY; without even the
#rec_double.c: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#rec_double.c: *                PARTICULAR PURPOSE.  See the GNU General Public
#rec_double.c: *                License for more details.
#rec_double.c: *
#rec_double.c: *                The GNU General Public License should be included with
#rec_double.c: *                this file.  If not, you can view it at
#rec_double.c: *                http://www.gnu.org/copyleft/gpl.html
#rec_double.c: *                or write to the Free Software Foundation, Inc., 59
#rec_double.c: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#rec_double.c: *
#rec_double.c: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#rec_double.c: *						and edit IJB, correctly.
#rec_double.c: *
#rec_double.c: * Revisions   :
#rec_double.c: *    $Log: contrib.sh,v $
#rec_double.c: *    Revision 1.3  2002/03/26 22:29:54  swa
#rec_double.c: *    we have a new homepage!
#rec_double.c: *
#rec_double.c: *    Revision 1.2  2002/03/24 13:25:43  swa
#rec_double.c: *    name change related issues
#rec_double.c: *
#rec_double.c: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#rec_double.c: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#rec_double.c: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#rec_double.c: *    contrib directory and I cannot upload a tarball ... it gets
#rec_double.c: *    corrupted).  This script will expand all files needed to create the
#rec_double.c: *    linked list modules and an example program.  Please see the README.
#rec_double.c: *    Feed back is welcomed.  Enjoy.
#rec_double.c: *
#rec_double.c: *
#rec_double.c: *********************************************************************/
#rec_double.c:
#rec_double.c:
#rec_double.c:#include <malloc.h>
#rec_double.c:#include <stdio.h>
#rec_double.c:#include <stdlib.h>
#rec_double.c:#include <string.h>
#rec_double.c:
#rec_double.c:#include "gen_list.h"
#rec_double.c:#include "rec_double.h"
#rec_double.c:
#rec_double.c:const char rec_double_h_rcs[] = REC_DOUBLE_H_VERSION;
#rec_double.c:
#rec_double.c:
#rec_double.c:static const rec_method rec_double_vtable[] =
#rec_double.c:{
#rec_double.c:	(rec_method)derived_rec_double_copy_construct,
#rec_double.c:	(rec_method)derived_rec_double_destruct,
#rec_double.c:	(rec_method)derived_rec_double_stream,
#rec_double.c:	(rec_method)derived_rec_double_equal
#rec_double.c:};
#rec_double.c:
#rec_double.c:
#rec_double.c:/*********************************************************************
#rec_double.c: *
#rec_double.c: * Function    :  derived_rec_double_construct
#rec_double.c: *
#rec_double.c: * Description :  A simple derived record class that contains 1 string.
#rec_double.c: *
#rec_double.c: * Parameters  :
#rec_double.c: *          1  :  The record
#rec_double.c: *          2  :  The string to contain.
#rec_double.c: *
#rec_double.c: * Returns     :  A pointer to the constructed record.
#rec_double.c: *
#rec_double.c: *********************************************************************/
#rec_double.c:struct derived_rec_double *derived_rec_double_construct( const double _contents )
#rec_double.c:{
#rec_double.c:	struct derived_rec_double *this_rec = (struct derived_rec_double *)gen_list_rec_construct(
#rec_double.c:		ISA_DOUBLE,
#rec_double.c:		sizeof( struct derived_rec_double ),
#rec_double.c:		rec_double_vtable
#rec_double.c:	);
#rec_double.c:
#rec_double.c:	this_rec->contents = _contents;
#rec_double.c:
#rec_double.c:	LIST_SHOW( printf( "\
#rec_double.c:\t\tdouble construct new rec\t\t\t\t= %p
#rec_double.c:\t\tdouble construct new rec contents\t= %8.2lf\n\n", (const void *)this_rec, this_rec->contents ) );
#rec_double.c:
#rec_double.c:	return( this_rec );
#rec_double.c:
#rec_double.c:}
#rec_double.c:
#rec_double.c:
#rec_double.c:/*********************************************************************
#rec_double.c: *
#rec_double.c: * Function    :  derived_rec_double_copy_construct
#rec_double.c: *
#rec_double.c: * Description :  Copies one double record to another.
#rec_double.c: *
#rec_double.c: * Parameters  :
#rec_double.c: *          1  :  Existing record.
#rec_double.c: *				2	:  Copy record.
#rec_double.c: *
#rec_double.c: * Returns     :  The newly constructed copy record.
#rec_double.c: *
#rec_double.c: *********************************************************************/
#rec_double.c:struct derived_rec_double *derived_rec_double_copy_construct( const struct derived_rec_double *this_rec )
#rec_double.c:{
#rec_double.c:	struct derived_rec_double *copy_rec = (struct derived_rec_double *)gen_list_rec_copy_construct( (struct gen_list_rec *)this_rec );
#rec_double.c:	copy_rec->contents = - this_rec->contents;
#rec_double.c:
#rec_double.c:	LIST_SHOW( printf( "\
#rec_double.c:\t\tdouble copy construct new gen rec\t\t\t\t= %p => %p
#rec_double.c:\t\tdouble copy construct new gen rec contents\t= %8.2lf\n\n", (const void *)this_rec, (const void *)copy_rec, copy_rec->contents ) );
#rec_double.c:
#rec_double.c:	return( copy_rec );
#rec_double.c:
#rec_double.c:}
#rec_double.c:
#rec_double.c:
#rec_double.c:/*********************************************************************
#rec_double.c: *
#rec_double.c: * Function    :  derived_rec_double_destruct	
#rec_double.c: *
#rec_double.c: * Description :  Destruct the double record.
#rec_double.c: *
#rec_double.c: * Parameters  :
#rec_double.c: *          1  :  The record.
#rec_double.c: *
#rec_double.c: * Returns     :  NULL
#rec_double.c: *
#rec_double.c: *********************************************************************/
#rec_double.c:struct derived_rec_double *derived_rec_double_destruct( struct derived_rec_double *this_rec )
#rec_double.c:{
#rec_double.c:	LIST_SHOW( printf( "\
#rec_double.c:\t\tdouble destruct this_rec\t\t\t\t= %p
#rec_double.c:\t\tdouble destruct this_rec->contents\t= %8.2lf\n\n", (const void *)this_rec, this_rec->contents ) );
#rec_double.c:
#rec_double.c:	this_rec->contents = -1.1111;
#rec_double.c:	return( (struct derived_rec_double *)gen_list_rec_destruct( (struct gen_list_rec *)this_rec ) );
#rec_double.c:
#rec_double.c:}
#rec_double.c:
#rec_double.c:
#rec_double.c:/*********************************************************************
#rec_double.c: *
#rec_double.c: * Function    :  derived_rec_double_stream
#rec_double.c: *
#rec_double.c: * Description :  Displays all double attributes on the STDOUT stream.
#rec_double.c: *
#rec_double.c: * Parameters  :
#rec_double.c: *          1  :  The record.
#rec_double.c: *
#rec_double.c: * Returns     :  The record.
#rec_double.c: *
#rec_double.c: *********************************************************************/
#rec_double.c:const struct derived_rec_double *derived_rec_double_stream( const struct derived_rec_double *this_rec )
#rec_double.c:{
#rec_double.c:	this_rec = (struct derived_rec_double *)gen_list_rec_stream(
#rec_double.c:		(struct gen_list_rec *)this_rec
#rec_double.c:	);
#rec_double.c:	LIST_SHOW( printf( "\
#rec_double.c:\t\tdouble stream this_rec\t\t\t\t= %p
#rec_double.c:\t\tdouble stream this_rec->contents\t= %8.2lf\n\n", (const void *)this_rec, this_rec->contents ) );
#rec_double.c:
#rec_double.c:	return( this_rec );
#rec_double.c:
#rec_double.c:}
#rec_double.c:
#rec_double.c:
#rec_double.c:/*********************************************************************
#rec_double.c: *
#rec_double.c: * Function    :  derived_rec_double_equal
#rec_double.c: *
#rec_double.c: * Description :  Compares two double records to see if they are equal.
#rec_double.c: *
#rec_double.c: * Parameters  :
#rec_double.c: *          1  :  A record.
#rec_double.c: *          2  :  Another record.
#rec_double.c: *
#rec_double.c: * Returns     :  0 => NOT EQUAL, anything else is EQUAL.
#rec_double.c: *
#rec_double.c: *********************************************************************/
#rec_double.c:int derived_rec_double_equal( const struct derived_rec_double *this_rec, const struct derived_rec_double *eq_rec )
#rec_double.c:{
#rec_double.c:	if ( ! gen_list_rec_equal( (const struct gen_list_rec *)this_rec, (struct gen_list_rec *)eq_rec ) )
#rec_double.c:	{
#rec_double.c:		return( 0 );
#rec_double.c:	}
#rec_double.c:	return( this_rec->contents == eq_rec->contents );
#rec_double.c:
#rec_double.c:}


#rec_double.h:#ifndef REC_DOUBLE_H_INCLUDED
#rec_double.h:#define REC_DOUBLE_H_INCLUDED
#rec_double.h:#define REC_DOUBLE_H_VERSION "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $"
#rec_double.h:/*********************************************************************
#rec_double.h: *
#rec_double.h: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#rec_double.h: *
#rec_double.h: * Purpose     :  gen_A "derived class" of gen_list_rec.
#rec_double.h: *
#rec_double.h: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#rec_double.h: *                Privoxy team. http://www.privoxy.org/
#rec_double.h: *
#rec_double.h: *                This program is free software; you can redistribute it
#rec_double.h: *                and/or modify it under the terms of the GNU General
#rec_double.h: *                Public License as published by the Free Software
#rec_double.h: *                Foundation; either version 2 of the License, or (at
#rec_double.h: *                your option) any later version.
#rec_double.h: *
#rec_double.h: *                This program is distributed in the hope that it will
#rec_double.h: *                be useful, but WITHOUT ANY WARRANTY; without even the
#rec_double.h: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#rec_double.h: *                PARTICULAR PURPOSE.  See the GNU General Public
#rec_double.h: *                License for more details.
#rec_double.h: *
#rec_double.h: *                The GNU General Public License should be included with
#rec_double.h: *                this file.  If not, you can view it at
#rec_double.h: *                http://www.gnu.org/copyleft/gpl.html
#rec_double.h: *                or write to the Free Software Foundation, Inc., 59
#rec_double.h: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#rec_double.h: *
#rec_double.h: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#rec_double.h: *						and edit IJB, correctly.
#rec_double.h: *
#rec_double.h: * Revisions   :
#rec_double.h: *    $Log: contrib.sh,v $
#rec_double.h: *    Revision 1.3  2002/03/26 22:29:54  swa
#rec_double.h: *    we have a new homepage!
#rec_double.h: *
#rec_double.h: *    Revision 1.2  2002/03/24 13:25:43  swa
#rec_double.h: *    name change related issues
#rec_double.h: *
#rec_double.h: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#rec_double.h: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#rec_double.h: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#rec_double.h: *    contrib directory and I cannot upload a tarball ... it gets
#rec_double.h: *    corrupted).  This script will expand all files needed to create the
#rec_double.h: *    linked list modules and an example program.  Please see the README.
#rec_double.h: *    Feed back is welcomed.  Enjoy.
#rec_double.h: *
#rec_double.h: *
#rec_double.h: *********************************************************************/
#rec_double.h:
#rec_double.h:
#rec_double.h:#ifdef __cplusplus
#rec_double.h:extern "C" {
#rec_double.h:#endif
#rec_double.h:
#rec_double.h:
#rec_double.h:struct derived_rec_double
#rec_double.h:{
#rec_double.h:	/* private: */
#rec_double.h:	struct gen_list_rec parent_rec;
#rec_double.h:	double contents;
#rec_double.h:};
#rec_double.h:
#rec_double.h:/* public: */
#rec_double.h:extern struct derived_rec_double *	derived_rec_double_construct( const double _contents );
#rec_double.h:extern struct derived_rec_double *	derived_rec_double_copy_construct( const struct derived_rec_double *this_rec );
#rec_double.h:extern struct derived_rec_double *	derived_rec_double_destruct( struct derived_rec_double *this_rec );
#rec_double.h:extern const struct derived_rec_double *derived_rec_double_stream( const struct derived_rec_double *this_rec );
#rec_double.h:extern int								derived_rec_double_equal( const struct derived_rec_double *this_rec, const struct derived_rec_double *eq_rec );
#rec_double.h:
#rec_double.h:/* struct/class COMPLETE */
#rec_double.h:
#rec_double.h:
#rec_double.h:#ifdef __cplusplus
#rec_double.h:} /* extern "C" */
#rec_double.h:#endif
#rec_double.h:
#rec_double.h:#endif /* ndef REC_DOUBLE_H_INCLUDED */
#rec_double.h:
#rec_double.h:/*
#rec_double.h:  Local Variables:
#rec_double.h:  tab-width: 3
#rec_double.h:  end:
#rec_double.h:*/


#rec_long.c:const char rec_long_rcs[] = "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $";
#rec_long.c:/*********************************************************************
#rec_long.c: *
#rec_long.c: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#rec_long.c: *
#rec_long.c: * Purpose     :  A "derived class" of gen_list_rec.
#rec_long.c: *
#rec_long.c: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#rec_long.c: *                Privoxy team. http://www.privoxy.org/
#rec_long.c: *
#rec_long.c: *                This program is free software; you can redistribute it
#rec_long.c: *                and/or modify it under the terms of the GNU General
#rec_long.c: *                Public License as published by the Free Software
#rec_long.c: *                Foundation; either version 2 of the License, or (at
#rec_long.c: *                your option) any later version.
#rec_long.c: *
#rec_long.c: *                This program is distributed in the hope that it will
#rec_long.c: *                be useful, but WITHOUT ANY WARRANTY; without even the
#rec_long.c: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#rec_long.c: *                PARTICULAR PURPOSE.  See the GNU General Public
#rec_long.c: *                License for more details.
#rec_long.c: *
#rec_long.c: *                The GNU General Public License should be included with
#rec_long.c: *                this file.  If not, you can view it at
#rec_long.c: *                http://www.gnu.org/copyleft/gpl.html
#rec_long.c: *                or write to the Free Software Foundation, Inc., 59
#rec_long.c: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#rec_long.c: *
#rec_long.c: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#rec_long.c: *						and edit IJB, correctly.
#rec_long.c: *
#rec_long.c: * Revisions   :
#rec_long.c: *    $Log: contrib.sh,v $
#rec_long.c: *    Revision 1.3  2002/03/26 22:29:54  swa
#rec_long.c: *    we have a new homepage!
#rec_long.c: *
#rec_long.c: *    Revision 1.2  2002/03/24 13:25:43  swa
#rec_long.c: *    name change related issues
#rec_long.c: *
#rec_long.c: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#rec_long.c: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#rec_long.c: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#rec_long.c: *    contrib directory and I cannot upload a tarball ... it gets
#rec_long.c: *    corrupted).  This script will expand all files needed to create the
#rec_long.c: *    linked list modules and an example program.  Please see the README.
#rec_long.c: *    Feed back is welcomed.  Enjoy.
#rec_long.c: *
#rec_long.c: *
#rec_long.c: *********************************************************************/
#rec_long.c:
#rec_long.c:
#rec_long.c:#include <malloc.h>
#rec_long.c:#include <stdio.h>
#rec_long.c:#include <stdlib.h>
#rec_long.c:#include <string.h>
#rec_long.c:
#rec_long.c:#include "gen_list.h"
#rec_long.c:#include "rec_long.h"
#rec_long.c:
#rec_long.c:const char rec_long_h_rcs[] = REC_LONG_H_VERSION;
#rec_long.c:
#rec_long.c:
#rec_long.c:static const rec_method rec_long_vtable[] =
#rec_long.c:{
#rec_long.c:	(rec_method)derived_rec_long_copy_construct,
#rec_long.c:	(rec_method)derived_rec_long_destruct,
#rec_long.c:	(rec_method)derived_rec_long_stream,
#rec_long.c:	(rec_method)derived_rec_long_equal
#rec_long.c:};
#rec_long.c:
#rec_long.c:
#rec_long.c:/*********************************************************************
#rec_long.c: *
#rec_long.c: * Function    :  derived_rec_long_construct
#rec_long.c: *
#rec_long.c: * Description :  A simple derived record class that contains 1 string.
#rec_long.c: *
#rec_long.c: * Parameters  :
#rec_long.c: *          1  :  The record
#rec_long.c: *          2  :  The string to contain.
#rec_long.c: *
#rec_long.c: * Returns     :  A pointer to the constructed record.
#rec_long.c: *
#rec_long.c: *********************************************************************/
#rec_long.c:struct derived_rec_long *derived_rec_long_construct( const long _contents )
#rec_long.c:{
#rec_long.c:	struct derived_rec_long *this_rec = (struct derived_rec_long *)gen_list_rec_construct(
#rec_long.c:		ISA_LONG,
#rec_long.c:		sizeof( struct derived_rec_long ),
#rec_long.c:		rec_long_vtable
#rec_long.c:	);
#rec_long.c:
#rec_long.c:	this_rec->contents = _contents;
#rec_long.c:
#rec_long.c:	LIST_SHOW( printf( "\
#rec_long.c:\t\tlong construct new rec\t\t\t\t= %p
#rec_long.c:\t\tlong construct new rec contents\t= %d\n\n", (const void *)this_rec, this_rec->contents ) );
#rec_long.c:
#rec_long.c:	return( this_rec );
#rec_long.c:
#rec_long.c:}
#rec_long.c:
#rec_long.c:
#rec_long.c:/*********************************************************************
#rec_long.c: *
#rec_long.c: * Function    :  derived_rec_long_copy_construct
#rec_long.c: *
#rec_long.c: * Description :  Copies one long record to another.
#rec_long.c: *
#rec_long.c: * Parameters  :
#rec_long.c: *          1  :  Existing record.
#rec_long.c: *				2	:  Copy record.
#rec_long.c: *
#rec_long.c: * Returns     :  The newly constructed copy record.
#rec_long.c: *
#rec_long.c: *********************************************************************/
#rec_long.c:struct derived_rec_long *derived_rec_long_copy_construct( const struct derived_rec_long *this_rec )
#rec_long.c:{
#rec_long.c:	struct derived_rec_long *copy_rec = (struct derived_rec_long *)gen_list_rec_copy_construct( (struct gen_list_rec *)this_rec );
#rec_long.c:	copy_rec->contents = - this_rec->contents;
#rec_long.c:
#rec_long.c:	LIST_SHOW( printf( "\
#rec_long.c:\t\tlong copy construct new gen rec\t\t\t\t= %p => %p
#rec_long.c:\t\tlong copy construct new gen rec contents\t= %d\n\n", (const void *)this_rec, (const void *)copy_rec, copy_rec->contents ) );
#rec_long.c:
#rec_long.c:	return( copy_rec );
#rec_long.c:
#rec_long.c:}
#rec_long.c:
#rec_long.c:
#rec_long.c:/*********************************************************************
#rec_long.c: *
#rec_long.c: * Function    :  derived_rec_long_destruct	
#rec_long.c: *
#rec_long.c: * Description :  Destruct the long record.
#rec_long.c: *
#rec_long.c: * Parameters  :
#rec_long.c: *          1  :  The record.
#rec_long.c: *
#rec_long.c: * Returns     :  NULL
#rec_long.c: *
#rec_long.c: *********************************************************************/
#rec_long.c:struct derived_rec_long *derived_rec_long_destruct( struct derived_rec_long *this_rec )
#rec_long.c:{
#rec_long.c:	LIST_SHOW( printf( "\
#rec_long.c:\t\tlong destruct this_rec\t\t\t\t= %p
#rec_long.c:\t\tlong destruct this_rec->contents\t= %d\n\n", (const void *)this_rec, this_rec->contents ) );
#rec_long.c:
#rec_long.c:	this_rec->contents = -1;
#rec_long.c:	return( (struct derived_rec_long *)gen_list_rec_destruct( (struct gen_list_rec *)this_rec ) );
#rec_long.c:
#rec_long.c:}
#rec_long.c:
#rec_long.c:
#rec_long.c:/*********************************************************************
#rec_long.c: *
#rec_long.c: * Function    :  derived_rec_long_stream
#rec_long.c: *
#rec_long.c: * Description :  Displays all long attributes on the STDOUT stream.
#rec_long.c: *
#rec_long.c: * Parameters  :
#rec_long.c: *          1  :  The record.
#rec_long.c: *
#rec_long.c: * Returns     :  The record.
#rec_long.c: *
#rec_long.c: *********************************************************************/
#rec_long.c:const struct derived_rec_long *derived_rec_long_stream( const struct derived_rec_long *this_rec )
#rec_long.c:{
#rec_long.c:	this_rec = (struct derived_rec_long *)gen_list_rec_stream(
#rec_long.c:		(struct gen_list_rec *)this_rec
#rec_long.c:	);
#rec_long.c:	LIST_SHOW( printf( "\
#rec_long.c:\t\tlong stream this_rec\t\t\t\t\t= %p
#rec_long.c:\t\tlong stream this_rec->contents\t= %d\n\n", (const void *)this_rec, this_rec->contents ) );
#rec_long.c:
#rec_long.c:	return( this_rec );
#rec_long.c:
#rec_long.c:}
#rec_long.c:
#rec_long.c:
#rec_long.c:/*********************************************************************
#rec_long.c: *
#rec_long.c: * Function    :  derived_rec_long_equal
#rec_long.c: *
#rec_long.c: * Description :  Compares two long records to see if they are equal.
#rec_long.c: *
#rec_long.c: * Parameters  :
#rec_long.c: *          1  :  A record.
#rec_long.c: *          2  :  Another record.
#rec_long.c: *
#rec_long.c: * Returns     :  0 => NOT EQUAL, anything else is EQUAL.
#rec_long.c: *
#rec_long.c: *********************************************************************/
#rec_long.c:int derived_rec_long_equal( const struct derived_rec_long *this_rec, const struct derived_rec_long *eq_rec )
#rec_long.c:{
#rec_long.c:	if ( ! gen_list_rec_equal( (const struct gen_list_rec *)this_rec, (struct gen_list_rec *)eq_rec ) )
#rec_long.c:	{
#rec_long.c:		return( 0 );
#rec_long.c:	}
#rec_long.c:	return( this_rec->contents == eq_rec->contents );
#rec_long.c:
#rec_long.c:}


#rec_long.h:#ifndef REC_LONG_H_INCLUDED
#rec_long.h:#define REC_LONG_H_INCLUDED
#rec_long.h:#define REC_LONG_H_VERSION "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $"
#rec_long.h:/*********************************************************************
#rec_long.h: *
#rec_long.h: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#rec_long.h: *
#rec_long.h: * Purpose     :  A "derived class" of gen_list_rec.
#rec_long.h: *
#rec_long.h: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#rec_long.h: *                Privoxy team. http://www.privoxy.org/
#rec_long.h: *
#rec_long.h: *                This program is free software; you can redistribute it
#rec_long.h: *                and/or modify it under the terms of the GNU General
#rec_long.h: *                Public License as published by the Free Software
#rec_long.h: *                Foundation; either version 2 of the License, or (at
#rec_long.h: *                your option) any later version.
#rec_long.h: *
#rec_long.h: *                This program is distributed in the hope that it will
#rec_long.h: *                be useful, but WITHOUT ANY WARRANTY; without even the
#rec_long.h: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#rec_long.h: *                PARTICULAR PURPOSE.  See the GNU General Public
#rec_long.h: *                License for more details.
#rec_long.h: *
#rec_long.h: *                The GNU General Public License should be included with
#rec_long.h: *                this file.  If not, you can view it at
#rec_long.h: *                http://www.gnu.org/copyleft/gpl.html
#rec_long.h: *                or write to the Free Software Foundation, Inc., 59
#rec_long.h: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#rec_long.h: *
#rec_long.h: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#rec_long.h: *						and edit IJB, correctly.
#rec_long.h: *
#rec_long.h: * Revisions   :
#rec_long.h: *    $Log: contrib.sh,v $
#rec_long.h: *    Revision 1.3  2002/03/26 22:29:54  swa
#rec_long.h: *    we have a new homepage!
#rec_long.h: *
#rec_long.h: *    Revision 1.2  2002/03/24 13:25:43  swa
#rec_long.h: *    name change related issues
#rec_long.h: *
#rec_long.h: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#rec_long.h: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#rec_long.h: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#rec_long.h: *    contrib directory and I cannot upload a tarball ... it gets
#rec_long.h: *    corrupted).  This script will expand all files needed to create the
#rec_long.h: *    linked list modules and an example program.  Please see the README.
#rec_long.h: *    Feed back is welcomed.  Enjoy.
#rec_long.h: *
#rec_long.h: *
#rec_long.h: *********************************************************************/
#rec_long.h:
#rec_long.h:
#rec_long.h:#ifdef __cplusplus
#rec_long.h:extern "C" {
#rec_long.h:#endif
#rec_long.h:
#rec_long.h:
#rec_long.h:struct derived_rec_long
#rec_long.h:{
#rec_long.h:	/* private: */
#rec_long.h:	struct gen_list_rec parent_rec;
#rec_long.h:	long contents;
#rec_long.h:};
#rec_long.h:
#rec_long.h:/* public: */
#rec_long.h:extern struct derived_rec_long *	derived_rec_long_construct( const long _contents );
#rec_long.h:extern struct derived_rec_long *	derived_rec_long_copy_construct( const struct derived_rec_long *this_rec );
#rec_long.h:extern struct derived_rec_long *	derived_rec_long_destruct( struct derived_rec_long *this_rec );
#rec_long.h:extern const struct derived_rec_long *derived_rec_long_stream( const struct derived_rec_long *this_rec );
#rec_long.h:extern int								derived_rec_long_equal( const struct derived_rec_long *this_rec, const struct derived_rec_long *eq_rec );
#rec_long.h:
#rec_long.h:/* struct/class COMPLETE */
#rec_long.h:
#rec_long.h:
#rec_long.h:#ifdef __cplusplus
#rec_long.h:} /* extern "C" */
#rec_long.h:#endif
#rec_long.h:
#rec_long.h:#endif /* ndef REC_LONG_H_INCLUDED */
#rec_long.h:
#rec_long.h:/*
#rec_long.h:  Local Variables:
#rec_long.h:  tab-width: 3
#rec_long.h:  end:
#rec_long.h:*/


#rec_malloc_police.c:const char rec_malloc_police_rcs[] = "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $";
#rec_malloc_police.c:/*********************************************************************
#rec_malloc_police.c: *
#rec_malloc_police.c: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#rec_malloc_police.c: *
#rec_malloc_police.c: * Purpose     :  A "derived class" of gen_list_rec.
#rec_malloc_police.c: *						This class helps to build a list of allocated and
#rec_malloc_police.c: *						freed memory.  When the program exits, we will print
#rec_malloc_police.c: *						a list of all memory that was allocated, but never
#rec_malloc_police.c: *						freed.  This could be most helpful to developers
#rec_malloc_police.c: *						and debugers.
#rec_malloc_police.c: *
#rec_malloc_police.c: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#rec_malloc_police.c: *                Privoxy team. http://www.privoxy.org/
#rec_malloc_police.c: *
#rec_malloc_police.c: *                This program is free software; you can redistribute it
#rec_malloc_police.c: *                and/or modify it under the terms of the GNU General
#rec_malloc_police.c: *                Public License as published by the Free Software
#rec_malloc_police.c: *                Foundation; either version 2 of the License, or (at
#rec_malloc_police.c: *                your option) any later version.
#rec_malloc_police.c: *
#rec_malloc_police.c: *                This program is distributed in the hope that it will
#rec_malloc_police.c: *                be useful, but WITHOUT ANY WARRANTY; without even the
#rec_malloc_police.c: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#rec_malloc_police.c: *                PARTICULAR PURPOSE.  See the GNU General Public
#rec_malloc_police.c: *                License for more details.
#rec_malloc_police.c: *
#rec_malloc_police.c: *                The GNU General Public License should be included with
#rec_malloc_police.c: *                this file.  If not, you can view it at
#rec_malloc_police.c: *                http://www.gnu.org/copyleft/gpl.html
#rec_malloc_police.c: *                or write to the Free Software Foundation, Inc., 59
#rec_malloc_police.c: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#rec_malloc_police.c: *
#rec_malloc_police.c: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#rec_malloc_police.c: *						and edit IJB, correctly.
#rec_malloc_police.c: *
#rec_malloc_police.c: * Revisions   :
#rec_malloc_police.c: *    $Log: contrib.sh,v $
#rec_malloc_police.c: *    Revision 1.3  2002/03/26 22:29:54  swa
#rec_malloc_police.c: *    we have a new homepage!
#rec_malloc_police.c: *
#rec_malloc_police.c: *    Revision 1.2  2002/03/24 13:25:43  swa
#rec_malloc_police.c: *    name change related issues
#rec_malloc_police.c: *
#rec_malloc_police.c: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#rec_malloc_police.c: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#rec_malloc_police.c: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#rec_malloc_police.c: *    contrib directory and I cannot upload a tarball ... it gets
#rec_malloc_police.c: *    corrupted).  This script will expand all files needed to create the
#rec_malloc_police.c: *    linked list modules and an example program.  Please see the README.
#rec_malloc_police.c: *    Feed back is welcomed.  Enjoy.
#rec_malloc_police.c: *
#rec_malloc_police.c: *
#rec_malloc_police.c: *********************************************************************/
#rec_malloc_police.c:
#rec_malloc_police.c:
#rec_malloc_police.c:#include <stdio.h>
#rec_malloc_police.c:
#rec_malloc_police.c:#include "gen_list.h"
#rec_malloc_police.c:#include "malloc_police.h"
#rec_malloc_police.c:#include "rec_malloc_police.h"
#rec_malloc_police.c:
#rec_malloc_police.c:const char rec_malloc_police_h_rcs[] = REC_MALLOC_POLICE_H_VERSION;
#rec_malloc_police.c:
#rec_malloc_police.c:
#rec_malloc_police.c:static const rec_method rec_malloc_police_vtable[] =
#rec_malloc_police.c:{
#rec_malloc_police.c:	(rec_method)derived_rec_malloc_police_copy_construct,
#rec_malloc_police.c:	(rec_method)derived_rec_malloc_police_destruct,
#rec_malloc_police.c:	(rec_method)derived_rec_malloc_police_stream,
#rec_malloc_police.c:	(rec_method)derived_rec_malloc_police_equal
#rec_malloc_police.c:};
#rec_malloc_police.c:
#rec_malloc_police.c:
#rec_malloc_police.c:/*********************************************************************
#rec_malloc_police.c: *
#rec_malloc_police.c: * Function    :  derived_rec_malloc_police_construct
#rec_malloc_police.c: *
#rec_malloc_police.c: * Description :  A simple derived record class that contains 1 string.
#rec_malloc_police.c: *
#rec_malloc_police.c: * Parameters  :
#rec_malloc_police.c: *          1  :  The record
#rec_malloc_police.c: *          2  :  The string to contain.
#rec_malloc_police.c: *
#rec_malloc_police.c: * Returns     :  A pointer to the constructed record.
#rec_malloc_police.c: *
#rec_malloc_police.c: *********************************************************************/
#rec_malloc_police.c:struct derived_rec_malloc_police *derived_rec_malloc_police_construct( void *_alloced_addr, char *_source_where, size_t _sz )
#rec_malloc_police.c:{
#rec_malloc_police.c:	struct derived_rec_malloc_police *this_rec;
#rec_malloc_police.c:	list_is_quiet ++;
#rec_malloc_police.c:
#rec_malloc_police.c:	this_rec = (struct derived_rec_malloc_police *)gen_list_rec_construct(
#rec_malloc_police.c:		ISA_MALLOC_POLICE,
#rec_malloc_police.c:		sizeof( struct derived_rec_malloc_police ),
#rec_malloc_police.c:		rec_malloc_police_vtable
#rec_malloc_police.c:	);
#rec_malloc_police.c:
#rec_malloc_police.c:	this_rec->alloced_addr	= _alloced_addr;
#rec_malloc_police.c:	this_rec->source_where	= STRDUP( _source_where );
#rec_malloc_police.c:	this_rec->sz				= _sz;
#rec_malloc_police.c:
#rec_malloc_police.c:/* 	LIST_SHOW( printf( "\ */
#rec_malloc_police.c:/* \t\tmalloc_police construct new rec\t\t\t\t\t= %p */
#rec_malloc_police.c:/* \t\tmalloc_police construct new rec alloced_addr = %p */
#rec_malloc_police.c:/* \t\tmalloc_police construct new rec source_where = %s */
#rec_malloc_police.c:/* \t\tmalloc_police construct new rec sz\t\t\t\t\t= %ld\n\n", */
#rec_malloc_police.c:/* 			  (const void *)this_rec, */
#rec_malloc_police.c:/* 			  this_rec->alloced_addr, */
#rec_malloc_police.c:/* 			  this_rec->source_where, */
#rec_malloc_police.c:/* 			  this_rec->sz */
#rec_malloc_police.c:/* 	) ); */
#rec_malloc_police.c:
#rec_malloc_police.c:	list_is_quiet --;
#rec_malloc_police.c:
#rec_malloc_police.c:	return( this_rec );
#rec_malloc_police.c:
#rec_malloc_police.c:}
#rec_malloc_police.c:
#rec_malloc_police.c:
#rec_malloc_police.c:/*********************************************************************
#rec_malloc_police.c: *
#rec_malloc_police.c: * Function    :  derived_rec_malloc_police_copy_construct
#rec_malloc_police.c: *
#rec_malloc_police.c: * Description :  Copies one malloc_police record to another.
#rec_malloc_police.c: *
#rec_malloc_police.c: * Parameters  :
#rec_malloc_police.c: *          1  :  Existing record.
#rec_malloc_police.c: *				2	:  Copy record.
#rec_malloc_police.c: *
#rec_malloc_police.c: * Returns     :  The newly constructed copy record.
#rec_malloc_police.c: *
#rec_malloc_police.c: *********************************************************************/
#rec_malloc_police.c:struct derived_rec_malloc_police *derived_rec_malloc_police_copy_construct( const struct derived_rec_malloc_police *this_rec )
#rec_malloc_police.c:{
#rec_malloc_police.c:	int len;
#rec_malloc_police.c:	char *new_contents;
#rec_malloc_police.c:	struct derived_rec_malloc_police *copy_rec;
#rec_malloc_police.c:
#rec_malloc_police.c:	list_is_quiet ++;
#rec_malloc_police.c:
#rec_malloc_police.c:	copy_rec = (struct derived_rec_malloc_police *)gen_list_rec_copy_construct( (struct gen_list_rec *)this_rec );
#rec_malloc_police.c:
#rec_malloc_police.c:	copy_rec->alloced_addr	= this_rec->alloced_addr;
#rec_malloc_police.c:	copy_rec->source_where	= STRDUP( this_rec->source_where );
#rec_malloc_police.c:	copy_rec->sz				= this_rec->sz;
#rec_malloc_police.c:
#rec_malloc_police.c:/* 	LIST_SHOW( printf( "\ */
#rec_malloc_police.c:/* \t\tmalloc_police copy construct new gen rec = %p => %p */
#rec_malloc_police.c:/* \t\tmalloc_police copy construct new gen rec alloced_addr = %p */
#rec_malloc_police.c:/* \t\tmalloc_police copy construct new gen rec source_where = %s */
#rec_malloc_police.c:/* \t\tmalloc_police copy construct new gen rec sz\t\t\t\t\t= %ld\n\n", */
#rec_malloc_police.c:/* 			  (const void *)this_rec, (const void *)copy_rec, */
#rec_malloc_police.c:/* 			  copy_rec->alloced_addr, */
#rec_malloc_police.c:/* 			  copy_rec->source_where, */
#rec_malloc_police.c:/* 			  copy_rec->sz */
#rec_malloc_police.c:/* 	) ); */
#rec_malloc_police.c:
#rec_malloc_police.c:	list_is_quiet --;
#rec_malloc_police.c:
#rec_malloc_police.c:	return( copy_rec );
#rec_malloc_police.c:
#rec_malloc_police.c:}
#rec_malloc_police.c:
#rec_malloc_police.c:
#rec_malloc_police.c:/*********************************************************************
#rec_malloc_police.c: *
#rec_malloc_police.c: * Function    :  derived_rec_malloc_police_destruct	
#rec_malloc_police.c: *
#rec_malloc_police.c: * Description :  Destruct the malloc_police record.
#rec_malloc_police.c: *
#rec_malloc_police.c: * Parameters  :
#rec_malloc_police.c: *          1  :  The record.
#rec_malloc_police.c: *
#rec_malloc_police.c: * Returns     :  NULL
#rec_malloc_police.c: *
#rec_malloc_police.c: *********************************************************************/
#rec_malloc_police.c:struct derived_rec_malloc_police *derived_rec_malloc_police_destruct( struct derived_rec_malloc_police *this_rec )
#rec_malloc_police.c:{
#rec_malloc_police.c:	struct derived_rec_malloc_police *d;
#rec_malloc_police.c:	list_is_quiet ++;
#rec_malloc_police.c:
#rec_malloc_police.c:/* 	LIST_SHOW( printf( "\ */
#rec_malloc_police.c:/* \t\tmalloc_police destruct this_rec\t\t\t\t\t\t= %p */
#rec_malloc_police.c:/* \t\tmalloc_police destruct this_rec->alloced_addr\t= %p */
#rec_malloc_police.c:/* \t\tmalloc_police destruct this_rec->source_where\t= %s, */
#rec_malloc_police.c:/* \t\tmalloc_police destruct this_rec->sz\t\t\t\t\t= %ld\n\n", */
#rec_malloc_police.c:/* 			  (const void *)this_rec, */
#rec_malloc_police.c:/* 			  this_rec->alloced_addr, */
#rec_malloc_police.c:/* 			  this_rec->source_where, */
#rec_malloc_police.c:/* 			  this_rec->sz */
#rec_malloc_police.c:/* 	) ); */
#rec_malloc_police.c:
#rec_malloc_police.c:	memset( this_rec->source_where, '!', strlen( this_rec->source_where ) );
#rec_malloc_police.c:	FREE( this_rec->source_where );
#rec_malloc_police.c:
#rec_malloc_police.c:	d = (struct derived_rec_malloc_police *)gen_list_rec_destruct( (struct gen_list_rec *)this_rec );
#rec_malloc_police.c:	list_is_quiet --;
#rec_malloc_police.c:
#rec_malloc_police.c:	return( d );
#rec_malloc_police.c:
#rec_malloc_police.c:}
#rec_malloc_police.c:
#rec_malloc_police.c:
#rec_malloc_police.c:/*********************************************************************
#rec_malloc_police.c: *
#rec_malloc_police.c: * Function    :  derived_rec_malloc_police_stream
#rec_malloc_police.c: *
#rec_malloc_police.c: * Description :  Displays all malloc_police attributes on the STDOUT stream.
#rec_malloc_police.c: *
#rec_malloc_police.c: * Parameters  :
#rec_malloc_police.c: *          1  :  The record.
#rec_malloc_police.c: *
#rec_malloc_police.c: * Returns     :  The record.
#rec_malloc_police.c: *
#rec_malloc_police.c: *********************************************************************/
#rec_malloc_police.c:const struct derived_rec_malloc_police *derived_rec_malloc_police_stream( const struct derived_rec_malloc_police *this_rec )
#rec_malloc_police.c:{
#rec_malloc_police.c:	list_is_quiet ++;
#rec_malloc_police.c:
#rec_malloc_police.c:	this_rec = (struct derived_rec_malloc_police *)gen_list_rec_stream(
#rec_malloc_police.c:		(struct gen_list_rec *)this_rec
#rec_malloc_police.c:	);
#rec_malloc_police.c:	LIST_SHOW( printf( "\
#rec_malloc_police.c:\t\tmalloc_police stream this_rec\t\t\t\t\t= %p
#rec_malloc_police.c:\t\tmalloc_police stream this_rec->alloced_addr\t= %p
#rec_malloc_police.c:\t\tmalloc_police stream this_rec->source_where\t= %s
#rec_malloc_police.c:\t\tmalloc_police stream this_rec->sz\t\t\t\t= %ld\n\n",
#rec_malloc_police.c:							 (const void *)this_rec,
#rec_malloc_police.c:							 this_rec->alloced_addr,
#rec_malloc_police.c:							 this_rec->source_where,
#rec_malloc_police.c:							 this_rec->sz
#rec_malloc_police.c:	) );
#rec_malloc_police.c:
#rec_malloc_police.c:	list_is_quiet --;
#rec_malloc_police.c:	return( this_rec );
#rec_malloc_police.c:
#rec_malloc_police.c:}
#rec_malloc_police.c:
#rec_malloc_police.c:
#rec_malloc_police.c:/*********************************************************************
#rec_malloc_police.c: *
#rec_malloc_police.c: * Function    :  derived_rec_malloc_police_equal
#rec_malloc_police.c: *
#rec_malloc_police.c: * Description :  Compares two malloc_police records to see if they are equal.
#rec_malloc_police.c: *
#rec_malloc_police.c: * Parameters  :
#rec_malloc_police.c: *          1  :  A record.
#rec_malloc_police.c: *          2  :  Another record.
#rec_malloc_police.c: *
#rec_malloc_police.c: * Returns     :  0 => NOT EQUAL, anything else is EQUAL.
#rec_malloc_police.c: *
#rec_malloc_police.c: *********************************************************************/
#rec_malloc_police.c:int derived_rec_malloc_police_equal( const struct derived_rec_malloc_police *this_rec, const struct derived_rec_malloc_police *eq_rec )
#rec_malloc_police.c:{
#rec_malloc_police.c:	list_is_quiet ++;
#rec_malloc_police.c:
#rec_malloc_police.c:	if ( ! gen_list_rec_equal( (const struct gen_list_rec *)this_rec, (struct gen_list_rec *)eq_rec ) )
#rec_malloc_police.c:	{
#rec_malloc_police.c:		return( 0 );
#rec_malloc_police.c:	}
#rec_malloc_police.c:
#rec_malloc_police.c:	list_is_quiet --;
#rec_malloc_police.c:	return( this_rec->alloced_addr == eq_rec->alloced_addr );
#rec_malloc_police.c:
#rec_malloc_police.c:}


#rec_malloc_police.h:#ifndef REC_MALLOC_POLICE_H_INCLUDED
#rec_malloc_police.h:#define REC_MALLOC_POLICE_H_INCLUDED
#rec_malloc_police.h:#define REC_MALLOC_POLICE_H_VERSION "$Id: contrib.sh,v 1.3 2002/03/26 22:29:54 swa Exp $"
#rec_malloc_police.h:/*********************************************************************
#rec_malloc_police.h: *
#rec_malloc_police.h: * File        :  $Source: /cvsroot/ijbswa/current/contrib.sh,v $
#rec_malloc_police.h: *
#rec_malloc_police.h: * Purpose     :  A "derived class" of gen_list_rec.
#rec_malloc_police.h: *						This class helps to build a list of allocated and
#rec_malloc_police.h: *						freed memory.  When the program exits, we will print
#rec_malloc_police.h: *						a list of all memory that was allocated, but never
#rec_malloc_police.h: *						freed.  This could be most helpful to developers
#rec_malloc_police.h: *						and debugers.
#rec_malloc_police.h: *
#rec_malloc_police.h: * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
#rec_malloc_police.h: *                Privoxy team. http://www.privoxy.org/
#rec_malloc_police.h: *
#rec_malloc_police.h: *                This program is free software; you can redistribute it
#rec_malloc_police.h: *                and/or modify it under the terms of the GNU General
#rec_malloc_police.h: *                Public License as published by the Free Software
#rec_malloc_police.h: *                Foundation; either version 2 of the License, or (at
#rec_malloc_police.h: *                your option) any later version.
#rec_malloc_police.h: *
#rec_malloc_police.h: *                This program is distributed in the hope that it will
#rec_malloc_police.h: *                be useful, but WITHOUT ANY WARRANTY; without even the
#rec_malloc_police.h: *                implied warranty of MERCHANTABILITY or FITNESS FOR A
#rec_malloc_police.h: *                PARTICULAR PURPOSE.  See the GNU General Public
#rec_malloc_police.h: *                License for more details.
#rec_malloc_police.h: *
#rec_malloc_police.h: *                The GNU General Public License should be included with
#rec_malloc_police.h: *                this file.  If not, you can view it at
#rec_malloc_police.h: *                http://www.gnu.org/copyleft/gpl.html
#rec_malloc_police.h: *                or write to the Free Software Foundation, Inc., 59
#rec_malloc_police.h: *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#rec_malloc_police.h: *
#rec_malloc_police.h: * VI users		:	Please "set tabstop=3 shiftwidth=3" to view this file,
#rec_malloc_police.h: *						and edit IJB, correctly.
#rec_malloc_police.h: *
#rec_malloc_police.h: * Revisions   :
#rec_malloc_police.h: *    $Log: contrib.sh,v $
#rec_malloc_police.h: *    Revision 1.3  2002/03/26 22:29:54  swa
#rec_malloc_police.h: *    we have a new homepage!
#rec_malloc_police.h: *
#rec_malloc_police.h: *    Revision 1.2  2002/03/24 13:25:43  swa
#rec_malloc_police.h: *    name change related issues
#rec_malloc_police.h: *
#rec_malloc_police.h: *    Revision 1.1  2001/12/07 01:54:50  iwanttokeepanon
#rec_malloc_police.h: *    A contribution/recomendation to the IJBSWA group for a generic doubly
#rec_malloc_police.h: *    linked list.  This file is a home brew "bash tar" (I cannot create a
#rec_malloc_police.h: *    contrib directory and I cannot upload a tarball ... it gets
#rec_malloc_police.h: *    corrupted).  This script will expand all files needed to create the
#rec_malloc_police.h: *    linked list modules and an example program.  Please see the README.
#rec_malloc_police.h: *    Feed back is welcomed.  Enjoy.
#rec_malloc_police.h: *
#rec_malloc_police.h: *
#rec_malloc_police.h: *********************************************************************/
#rec_malloc_police.h:
#rec_malloc_police.h:
#rec_malloc_police.h:#ifdef __cplusplus
#rec_malloc_police.h:extern "C" {
#rec_malloc_police.h:#endif
#rec_malloc_police.h:
#rec_malloc_police.h:
#rec_malloc_police.h:struct derived_rec_malloc_police
#rec_malloc_police.h:{
#rec_malloc_police.h:	/* private: */
#rec_malloc_police.h:	struct	gen_list_rec parent_rec;
#rec_malloc_police.h:	void		*alloced_addr;
#rec_malloc_police.h:	char		*source_where;
#rec_malloc_police.h:	size_t	sz;
#rec_malloc_police.h:};
#rec_malloc_police.h:
#rec_malloc_police.h:/* public: */
#rec_malloc_police.h:extern struct derived_rec_malloc_police *	derived_rec_malloc_police_construct( void *_alloced_addr, char *_source_where, size_t _sz );
#rec_malloc_police.h:extern struct derived_rec_malloc_police *	derived_rec_malloc_police_copy_construct( const struct derived_rec_malloc_police *this_rec );
#rec_malloc_police.h:extern struct derived_rec_malloc_police *	derived_rec_malloc_police_destruct( struct derived_rec_malloc_police *this_rec );
#rec_malloc_police.h:extern const struct derived_rec_malloc_police *derived_rec_malloc_police_stream( const struct derived_rec_malloc_police *this_rec );
#rec_malloc_police.h:extern int											derived_rec_malloc_police_equal( const struct derived_rec_malloc_police *this_rec, const struct derived_rec_malloc_police *eq_rec );
#rec_malloc_police.h:
#rec_malloc_police.h:/* struct/class COMPLETE */
#rec_malloc_police.h:
#rec_malloc_police.h:
#rec_malloc_police.h:#ifdef __cplusplus
#rec_malloc_police.h:} /* extern "C" */
#rec_malloc_police.h:#endif
#rec_malloc_police.h:
#rec_malloc_police.h:
#rec_malloc_police.h:#endif /* ndef REC_MALLOC_POLICE_H_INCLUDED */
#rec_malloc_police.h:
#rec_malloc_police.h:/*
#rec_malloc_police.h:  Local Variables:
#rec_malloc_police.h:  tab-width: 3
#rec_malloc_police.h:  end:
#rec_malloc_police.h:*/


###########################################################################
##  Description:			makeInsertableFile
##		Use this function to make a file suitable for insertion into this
##		script.  This file (x), will contain all of the files listed.
##
##  Inputs :
##		None
##
##  Outputs :
##		Return status:	(0) - Everything is Okay, anything else is an error
###########################################################################

function makeInsertableFile {

	rm -f x

	for i in ${fileList}; do
		sed -e "s/^/#${i}:/" ${i} >> x
		echo "" >> x
		echo "" >> x
	done

	return 0

}


#
#Local Variables:
#tab-width: 3
#end:
#
