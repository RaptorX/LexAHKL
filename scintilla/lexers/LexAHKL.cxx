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

	CharacterSet valLabel;
	CharacterSet valIdentifier;
	CharacterSet valHotstringOpt;

	WordList directives;
	WordList commands;
	WordList parameters;
	WordList flow;
	WordList functions;
	WordList variables;
	WordList keys;
	WordList user1;
	WordList user2;

	CharacterSet ExpOperator;
	CharacterSet SynOperator;
	CharacterSet EscSequence;
	
public:
	LexerAHKL() :
	valIdentifier(CharacterSet::setAlphaNum, "@#$_"),
	valLabel(CharacterSet::setAlphaNum, "@#$_~!^&*()+[]\';./\\<>?|{}-=\""),
	valHotstringOpt(CharacterSet::setDigits, "*?BbCcEeIiKkOoPpRrSsZz"),
	ExpOperator(CharacterSet::setNone, "+-*/!~&|^<>.:"),
	SynOperator(ExpOperator, "()[]?,{}"),
	EscSequence(CharacterSet::setNone, ",%`;nrbtvaf"){
	}

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

	// non-lexical states
	bool OnlySpaces;
	bool hasAlpha;
	
	bool inCommand;
	bool inHotstring;
	bool inExpression;
	bool inStringBlk = (sc.state == SCE_AHKL_STRINGOPTS || sc.state == SCE_AHKL_STRINGBLOCK || sc.state == SCE_AHKL_STRINGCOMMENT);
	bool inCommentBlk = (sc.state == SCE_AHKL_COMMENTDOC || sc.state == SCE_AHKL_COMMENTBLOCK);
	
	for (; sc.More(); sc.Forward()){

		// AutoHotkey usually resets lexical state in a per line base except in Comment and String Blocks
		if (sc.atLineStart) {
			OnlySpaces = true, hasAlpha = false, validLabel = true, validIdentifier = true,
			inCommand = false, inHotstring = false, inExpression = false;

			if (!inStringBlk && !inCommentBlk)
				sc.SetState(SCE_AHKL_NEUTRAL);
		}
		
		// Exit Current State
		switch (sc.state) {}

		// Enter New State
		if (sc.state == SCE_AHKL_NEUTRAL) {}

		if (!isspace(sc.ch))
			OnlySpaces = false;
		else
			hasAlpha = false;								// Make sure we clear the alpha flag for later decimal numbers
	}

	sc.Complete();
}

void SCI_METHOD LexerAHKL::Fold(unsigned int startPos, int length, int initStyle, IDocument *pAccess)
{
}

LexerModule lmAHKL(SCLEX_AHKL, LexerAHKL::LexerFactory, "ahkl", ahklWordLists);