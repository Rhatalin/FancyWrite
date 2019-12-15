#include "FancyWrite.h"
#include <vector>
#include <iostream>


FancyWrite::FancyWrite()
	: hConsole{ GetStdHandle(STD_OUTPUT_HANDLE) }, originalConsoleForeground{ 15 }, CSBI{}
{
	SetConsoleScreenBufferSize(hConsole, COORD{ 200, 200 });
	// store current console color
	if (GetConsoleScreenBufferInfo(hConsole, &CSBI)) {
		originalConsoleForeground = CSBI.wAttributes;
	}
}

FancyWrite::~FancyWrite()
{
	// set console color to original
	SetConsoleTextAttribute(hConsole, originalConsoleForeground);
}

void FancyWrite::changeColor(FancyColor color)
{
	SetConsoleTextAttribute(hConsole, (int)color);
}

void FancyWrite::write(std::string text, FancyColor color)
{
	// store current console color
	WORD lastColor{ 15 };
	if (GetConsoleScreenBufferInfo(hConsole, &CSBI)) {
		lastColor = CSBI.wAttributes;
	}

	changeColor(color);
	std::cout << text << std::flush;

	// set color to last color
	SetConsoleTextAttribute(hConsole, lastColor);
}

void FancyWrite::writeRepeated(std::string text, unsigned int count, FancyColor color)
{
	// store current console color
	WORD lastColor{ 15 };
	if (GetConsoleScreenBufferInfo(hConsole, &CSBI)) {
		lastColor = CSBI.wAttributes;
	}

	changeColor(color);
	for (unsigned int i{ 0 }; i < count; i++) {
		std::cout << text << std::flush;
	}

	// set color to last color
	SetConsoleTextAttribute(hConsole, lastColor);
}

void FancyWrite::writeWrapped(std::string text, char borderChar, FancyColor colorText, FancyColor colorBorder)
{
	// store current console color
	WORD lastColor{ 15 };
	if (GetConsoleScreenBufferInfo(hConsole, &CSBI)) {
		lastColor = CSBI.wAttributes;
	}
	// split text into rows and remember the maximum letter count in a row
	std::vector<std::string> rows{};
	std::string row{};
	unsigned int maxRowLetters{ 0 };
	for (unsigned int iLetter{ 0 }; iLetter < text.length(); iLetter++) {
		// store row characters into row except EOL
		if (text[iLetter] != '\n') {
			row += text[iLetter];
		}
		// store complete row when EOL or end of string
		if (text[iLetter] == '\n' || iLetter == text.length() - 1) {
			rows.push_back(row);
			if (maxRowLetters < row.length()) {
				maxRowLetters = row.length();
			}
			row = "";
		}
	}

	// wrap text
	changeColor(colorBorder);
	for (unsigned int iLetter{ 0 }; iLetter < maxRowLetters + 4; iLetter++) {
		std::cout << borderChar << std::flush;
	}
	std::cout << "\n" << std::flush;
	for (unsigned int iRow{ 0 }; iRow < rows.size(); iRow++) {
		std::cout << borderChar << std::flush;
		std::cout << " " << std::flush;
		changeColor(colorText);
		for (unsigned int iLetter{ 0 }; iLetter < maxRowLetters; iLetter++) {
			if (iLetter < rows[iRow].length()) {
				std::cout << rows[iRow][iLetter] << std::flush;
			}
			else {
				std::cout << " " << std::flush;
			}
		}
		std::cout << " " << std::flush;
		changeColor(colorBorder);
		std::cout << borderChar << std::flush;
		std::cout << "\n" << std::flush;
	}
	for (unsigned int iLetter{ 0 }; iLetter < maxRowLetters + 4; iLetter++) {
		std::cout << borderChar << std::flush;
	}
	// set color to last color
	SetConsoleTextAttribute(hConsole, lastColor);
}
