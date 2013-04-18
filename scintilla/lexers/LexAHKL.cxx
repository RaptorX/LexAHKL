// Scintilla source code edit control
/** @file LexAHKL.cxx
 ** Lexer AutoHotkey L
 ** Created by Isaias "RaptorX" Baez (graptorx@gmail.com)
 **/
// Copyright ©2013 Isaias "RaptorX" Baez <graptorx@gmail.com> - [GPLv3]
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <string>
#include <sstream>

#include <vector>
#include <map>

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "CharacterSet.h"
#include "LexerModule.h"
#include <windows.h>

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

static const char *const ahklWordLists[] = {
						"Directives",
						"Commands",
						"Command Parameters",
						"Control Flow",
						"Built-in Functions",
						"Built-in Variables",
						"Keyboard & Mouse Keys",
						"User Defined 1",
						"User Defined 2",
					   0};

class LexerAHKL : public ILexer {

public:
	LexerAHKL(){}

	virtual ~LexerAHKL() {
	}
	void SCI_METHOD Release() {
		delete this;
	}
	int SCI_METHOD Version() const {
		return lvOriginal;
	}
	const char * SCI_METHOD PropertyNames() {
		return "";
	}
	int SCI_METHOD PropertyType(const char *name) {
		return 0;
	}
	const char * SCI_METHOD DescribeProperty(const char *name) {
		return "";
	}
	int SCI_METHOD PropertySet(const char *key, const char *val) {
		return 0;
	}
	const char * SCI_METHOD DescribeWordListSets() {
		return 0;
	}
	void * SCI_METHOD PrivateCall(int, void *) {
		return 0;
	}
	static ILexer *LexerFactory() {
		return new LexerAHKL();
	}

	int SCI_METHOD WordListSet(int n, const char *wl);
	void SCI_METHOD Lex(unsigned int startPos, int length, int initStyle, IDocument *pAccess);
	void SCI_METHOD Fold(unsigned int startPos, int length, int initStyle, IDocument *pAccess);
};

int SCI_METHOD LexerAHKL::WordListSet(int n, const char *wl)
{
	return 0;
}

void SCI_METHOD LexerAHKL::Lex(unsigned int startPos, int length, int initStyle, IDocument *pAccess)
{
	LexAccessor styler(pAccess);
	StyleContext sc(startPos, length, initStyle, styler);

	for (; sc.More(); sc.Forward()){

		// Exit Current State
		switch (sc.state) {}

		// Enter New State
		if (sc.state == SCE_AHKL_NEUTRAL) {}

	}

	sc.Complete();
}

void SCI_METHOD LexerAHKL::Fold(unsigned int startPos, int length, int initStyle, IDocument *pAccess)
{
}

LexerModule lmAHKL(SCLEX_AHKL, LexerAHKL::LexerFactory, "ahkl", ahklWordLists);