package main

import (
	"encoding/binary"
	"errors"
	"fmt"
	"os"
)

var sBoxes = [8][16]uint8{
	{4, 10, 9, 2, 13, 8, 0, 14, 6, 11, 1, 12, 7, 15, 5, 3},
	{14, 11, 4, 12, 6, 13, 15, 10, 2, 3, 8, 1, 0, 7, 5, 9},
	{5, 8, 1, 13, 10, 3, 4, 2, 14, 15, 12, 7, 6, 0, 9, 11},
	{7, 13, 10, 1, 0, 8, 9, 15, 14, 4, 6, 12, 11, 2, 5, 3},
	{6, 12, 7, 1, 5, 15, 13, 8, 4, 10, 9, 14, 0, 3, 11, 2},
	{4, 11, 10, 0, 7, 2, 1, 13, 3, 6, 8, 5, 9, 12, 15, 14},
	{13, 11, 4, 1, 3, 15, 5, 9, 0, 10, 14, 7, 6, 8, 2, 12},
	{1, 15, 13, 0, 5, 7, 10, 4, 9, 2, 3, 14, 6, 11, 8, 12},
}

func f(block uint32, key uint32) uint32 {
	sum := block + key
	for i := 0; i < 8; i++ {
		sum = (sum &^ (15 << (4 * i))) | (uint32(sBoxes[i][(sum>>(4*i))&15]) << (4 * i))
	}
	return (sum << 11) | (sum >> 21)
}

func encryptBlock(block *[2]uint32, key []uint32) {
	a, b := block[0], block[1]
	for i := 0; i < 32; i++ {
		t := b ^ f(a, key[i%8])
		b = a
		a = t
	}
	block[0], block[1] = b, a
}

func decryptBlock(block *[2]uint32, key []uint32) {
	a, b := block[0], block[1]
	for i := 31; i >= 0; i-- {
		t := b ^ f(a, key[i%8])
		b = a
		a = t
	}
	block[0], block[1] = b, a
}

func SimpleReplacementModeEnc(data []byte, key []uint32) error {
	if len(data)%8 != 0 {
		return errors.New("invalid data length")
	}

	temp := make([]byte, 8)
	for i := 0; i < len(data); i += 8 {
		block := [2]uint32{binary.LittleEndian.Uint32(data[i : i+4]), binary.LittleEndian.Uint32(data[i+4 : i+8])}
		encryptBlock(&block, key)
		binary.LittleEndian.PutUint32(temp[0:4], block[0])
		binary.LittleEndian.PutUint32(temp[4:8], block[1])
		copy(data[i:i+8], temp)
	}

	return nil
}

func SimpleReplacementModeDec(data []byte, key []uint32) error {
	if len(data)%8 != 0 {
		return errors.New("invalid data length")
	}

	temp := make([]byte, 8)
	for i := 0; i < len(data); i += 8 {
		block := [2]uint32{binary.LittleEndian.Uint32(data[i : i+4]), binary.LittleEndian.Uint32(data[i+4 : i+8])}
		decryptBlock(&block, key)
		binary.LittleEndian.PutUint32(temp[0:4], block[0])
		binary.LittleEndian.PutUint32(temp[4:8], block[1])
		copy(data[i:i+8], temp)
	}

	return nil
}

func main() {
	var err error
	defer func() {
		if err != nil {
			fmt.Println(err.Error())
		}
	}()

	key := []uint32{
		1234, 2345, 3456, 4567,
		5678, 6789, 7890, 8901,
	}

	inputFilename := "input.txt"
	encryptedFilename := "enc.txt"
	decryptedFilename := "dec.txt"

	data, err := os.ReadFile(inputFilename)
	if err != nil {
		return
	}

	err = SimpleReplacementModeEnc(data, key)
	if err != nil {
		return
	}

	err = os.WriteFile(encryptedFilename, data, 0644)
	if err != nil {
		return
	}

	err = SimpleReplacementModeDec(data, key)

	err = os.WriteFile(decryptedFilename, data, 0644)
}
