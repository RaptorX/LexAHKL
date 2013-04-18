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
	SynOperator(CharacterSet::setNone, "+-*/!~&|^<>.:()[]?,{}"),
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
	WordList *wordListN = 0;
	switch (n) {

		case 0:
			wordListN = &directives;
		break;

		case 1:
			wordListN = &commands;
		break;

		case 2:
			wordListN = &parameters;
		break;

		case 3:
			wordListN = &flow;
		break;

		case 4:
			wordListN = &functions;
		break;

		case 5:
			wordListN = &variables;
		break;

		case 6:
			wordListN = &keys;
		break;

		case 7:
			wordListN = &user1;
		break;

		case 8:
			wordListN = &user2;
		break;

	}

	int firstModification = -1;
	if (wordListN) {
		WordList wlNew;
		wlNew.Set(wl);
		if (*wordListN != wlNew) {
			wordListN->Set(wl);
			firstModification = 0;
		}
	}
	return firstModification;
}

void SCI_METHOD LexerAHKL::Lex(unsigned int startPos, int length, int initStyle, IDocument *pAccess)
{
	LexAccessor styler(pAccess);
	StyleContext sc(startPos, length, initStyle, styler);

	// non-lexical states
	bool OnlySpaces;
	bool hasAlpha;

	bool inKey;
	bool inCommand;
	bool inHotstring;
	bool inExpression;
	bool inStringBlk = (sc.state == SCE_AHKL_STRINGOPTS || sc.state == SCE_AHKL_STRINGBLOCK || sc.state == SCE_AHKL_STRINGCOMMENT);
	bool inCommentBlk = (sc.state == SCE_AHKL_COMMENTDOC || sc.state == SCE_AHKL_COMMENTBLOCK);

	for (; sc.More(); sc.Forward()){

		// AutoHotkey usually resets lexical state in a per line base except in Comment and String Blocks
		if (sc.atLineStart) {
			OnlySpaces = true, hasAlpha = false, inKey = false,
			inCommand = false, inHotstring = false, inExpression = false;

			if (!inStringBlk && !inCommentBlk)
				sc.SetState(SCE_AHKL_NEUTRAL);
		}

		// Exit Current State
		switch (sc.state) {

			case SCE_AHKL_IDENTIFIER: 	{
				if (sc.atLineEnd || !valIdentifier.Contains(sc.ch)) {			// Check for match after typing whole words or punctuation signs

					char identifier[256];
					sc.GetCurrentLowered(identifier, sizeof(identifier));

					if (directives.InList(identifier)) {

						sc.ChangeState(SCE_AHKL_DIRECTIVE);

					} else if (!inExpression && !inCommand && !inKey
						&& sc.ch != '(' && commands.InList(identifier)) {

						inCommand = true;
						sc.ChangeState(SCE_AHKL_COMMAND);

					} else if (inCommand && parameters.InList(identifier)) {

						sc.ChangeState(SCE_AHKL_PARAM);

					} else if (!inKey && flow.InList(identifier)) {			// avoid conflicts with key identifiers (e.g. pause)

						sc.ChangeState(SCE_AHKL_CONTROLFLOW);

					} else if (sc.ch == '(' && functions.InList(identifier)) {

						inCommand = true;
						sc.ChangeState(SCE_AHKL_BUILTINFUNCTION);

					} else if (variables.InList(identifier)) {

						sc.ChangeState(SCE_AHKL_BUILTINVAR);

					} else if (inKey && keys.InList(identifier)) {

						sc.ChangeState(SCE_AHKL_KEY);

					} else if (user1.InList(identifier)) {

						sc.ChangeState(SCE_AHKL_USERDEFINED1);

					} else if (user2.InList(identifier)) {

						sc.ChangeState(SCE_AHKL_USERDEFINED2);

					}

					sc.SetState(SCE_AHKL_NEUTRAL);

				}
			break;
			}

			case SCE_AHKL_COMMENTDOC:	{
				if (OnlySpaces && sc.Match('*','/')){

					inCommentBlk = false;
					sc.Forward(2);
					sc.SetState(SCE_AHKL_NEUTRAL);

				} else if ((OnlySpaces || isspace(sc.chPrev)) && sc.Match('@')) {

					sc.SetState(SCE_AHKL_COMMENTKEYWORD);

				}
			break;
			}

			case SCE_AHKL_COMMENTKEYWORD:	{
				if (sc.Match(':'))
					sc.ForwardSetState(SCE_AHKL_COMMENTDOC);
			break;
			}

			case SCE_AHKL_COMMENTBLOCK:	{
				if (OnlySpaces && sc.Match('*','/')){

					inCommentBlk = false;
					sc.Forward(2);
					sc.SetState(SCE_AHKL_NEUTRAL);

				}
			break;
			}

		}

		// Enter New State
		if (sc.state == SCE_AHKL_NEUTRAL) {

			if (!sc.atLineEnd && valIdentifier.Contains(sc.ch)) {
			
				sc.SetState(SCE_AHKL_IDENTIFIER);
				
			} else if (sc.ch == ')') {
			
				inCommand = false;
				
			} else if (sc.ch == '{') {
				
				inKey = true;
				
			} else if (sc.ch == '}') {
			
				inKey = false;

			} else if ((OnlySpaces || isspace(sc.chPrev)) && sc.Match(';')) {

				sc.SetState(SCE_AHKL_COMMENTLINE);

			} else if (OnlySpaces && sc.Match('/', '*')) {

				inCommentBlk = true;
				sc.ChangeState(SCE_AHKL_COMMENTBLOCK);

				if (sc.Match("/**") && !sc.Match("/***"))
					sc.ChangeState(SCE_AHKL_COMMENTDOC);

			}


		}

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