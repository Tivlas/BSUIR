package main

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/scanner"
	"go/token"
	"os"
)

func handleError(pos token.Position, msg string) {
	fmt.Printf("Error: %s: %s\n", pos, msg)
}

func lexical(filename string) {
	source, err := os.ReadFile(filename)
	if err != nil {
		fmt.Printf("Failed to read file: %v\n", err)
		return
	}
	var s scanner.Scanner

	fset := token.NewFileSet()
	file := fset.AddFile(filename, fset.Base(), len(source))

	s.Init(file, source, handleError, scanner.ScanComments)
	for {
		pos, tok, lit := s.Scan()
		if tok == token.EOF {
			fmt.Println("<EOF>")
			break
		}
		position := fset.Position(pos)
		position.Filename = ""
		fmt.Printf("%s\t%s\t%q\n", position, tok, lit)
	}
}

func syntax(filename string) {
	source, err := os.ReadFile(filename)
	if err != nil {
		fmt.Printf("Failed to read file: %v\n", err)
		return
	}

	fset := token.NewFileSet()
	file, err := parser.ParseFile(fset, filename, source, parser.ParseComments)
	if err != nil {
		if errList, ok := err.(scanner.ErrorList); ok {
			for _, e := range errList {
				fmt.Println(e)
			}
		}
		fmt.Printf("Failed to parse file: %v\n", err)
		return
	}

	// Process the parsed AST
	// ...

	// Example: Print the AST
	ast.Print(fset, file)
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: go run main.go <filename>")
		return
	}
	filename := os.Args[1]
	lexical(filename)
	fmt.Println("\n--------------------------")
	syntax(filename)
}
