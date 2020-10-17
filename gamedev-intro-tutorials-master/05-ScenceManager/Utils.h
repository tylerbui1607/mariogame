#pragma once

#include <Windows.h>
#include<iostream>
#include <signal.h>

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include<string.h>

using namespace std;

void DebugOut(wchar_t *fmt, ...);

vector<string> split(string line, string delimeter = "\t");
wstring ToWSTR(string st);

LPCWSTR ToLPCWSTR(string st);

