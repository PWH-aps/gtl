//////////////////////////////////////////////////////////////////////
//
// gtl.h:
//
// PWH
//    2020.11.12
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "gtl/_default.h"
#include "gtl/_lib_gtl.h"
#include "gtl/concepts.h"
#include "gtl/misc.h"
#include "gtl/string.h"
#include "gtl/archive.h"
#include "gtl/reflection.h"
//#include "gtl/dynamic.h"
#include "gtl/unit.h"
#include "gtl/coord.h"
#include "gtl/rand.h"
#include "gtl/mat_helper.h"


// todo : streamable_unique_ptr / streamable_shared_ptr - derived from std::unique_ptr / std::shared_ptr  respectively. for dynamic creatable object


//-----------------------------------------------------------------------------
// Naming Convention :
/*

* basic : maximize the utilities of IDE
	+ class Name
		- camal case
		- class name starts with 'x'
			ex) xCoordTrans
		- template class name starts with 'T'
		- interface class name starts with 'I'
		- Abbrevs. are all in UPPER CASE.
		- Function Starts with Upper Case.

	+ class member variables :
		i) public member variables - starts with 'm_'
		ii) private members ....... ???

	+ class member functions : ... which to take?
		i) Categorizing - Category comes first ?
			ex) functions with 'Path'
				Path + Get -> PathGet, PathSet, PathInit ...
		ii) Verb + Object.
			ex) functions with 'Path'
				GetPath, SetPath, InitPath ...

		ex)
			PutSomethingSpecial() ('P', 'S', 'S' �빮�� ���)
			GetDNS_Name : Get, DNS, Name �� 'G'et, 'DNS', 'N'ame �� �빮�� ���. DNS �� Name �� �빮�ڷ� ���� �ǹǷ�, '_' �� ����Ͽ� �����.

ex)
	(1) (Set/Get), (Read/Write), (Send/Receive)
		- Set/Get : �ܼ��� ������ ���� ���� �� ���.
		- Read/Write : File/Stream ���� �����. IO ���̰� ������ ���.
		- Send/Receive : Ư�� Protocol �� ����Ͽ� �����͸� ��/���� �ؾ� �ϴ� ���.

	(2) Init, Clear
		- Init() : �ʱ�ȭ. Class �� ����ϱ� ��, � �۾�(���� ������̵�, ...)�� �ؾ� �� ���.
		- Clear() : �ʱ�ȭ. �ܼ� ������ 0���� ������ �۾�.

	(3) FromString, ToString



*/
