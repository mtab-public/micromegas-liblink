(* ::Package:: *)

Quit[]


path = NotebookDirectory[];


(* ::Subsubsection:: *)
(*create micromegas lib*)


Needs["CCompilerDriver`"]


src = path <> "micromegas_dll.c"


myLib = CreateLibrary[{src}, "micromegas", "LinkerOptions"->"-luuid"]


(* ::Subsubsection:: *)
(*load micromegas lib*)


Clear[omega]

omega = LibraryFunctionLoad["micromegas", "call_micromegas_c_no_io", {{Real, 1}}, Real]
