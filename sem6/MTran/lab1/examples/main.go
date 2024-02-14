package main

import (
	"examples/example1"
	"examples/example2"
	"fmt"
	"log"
)

func main() {
	circle := example2.Circle{Radius: 5}
	rectangle := example2.Rectangle{Width: 4, Height: 6}

	shapes := []example2.Shape{circle, rectangle}

	for _, shape := range shapes {
		switch shape.(type) {
		case example2.Circle:
			fmt.Println("Circle")
		case example2.Rectangle:
			fmt.Println("Rectangle")
		}

		area := shape.Area()
		fmt.Printf("Area: %.2f\n", area)

		if area > 20 {
			break
		}

		if area < 10 {
			continue
		}
	}

	max := example1.Max(10, 20)
	fmt.Println("Max:", max)

	counts := example1.CountChars("hello")
	fmt.Println("Character Counts:", counts)

	slice := []int{5, 8, 3, 12, 6}
	maxNum, err := example1.SliceMax(slice)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("Maximum Number in Slice:", maxNum)

	twoLargest, err := example1.TwoLargestNums(slice)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("Two Largest Numbers in Slice:", twoLargest)
}
