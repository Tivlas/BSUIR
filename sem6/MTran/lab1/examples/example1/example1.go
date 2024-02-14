package example1

import "errors"

func Max(a, b int) int { // вместо int любой числовой тип
	if a > b {
		return a
	}
	return b
}

func CountChars(s string) map[rune]int {
	cnt := make(map[rune]int, 0)
	for _, rn := range s {
		cnt[rn]++
	}
	return cnt
}

func SliceMax(sl []int) (int, error) {
	if len(sl) > 0 {
		mx := sl[0]
		for i := 0; i < len(sl); i++ {
			mx = Max(mx, sl[i])
		}
		return mx, nil
	}
	return -1, errors.New("Slice was empty!")
}

func TwoLargestNums(sl []int) ([2]int, error) {
	if len(sl) < 2 {
		return [2]int{}, errors.New("Slice len < 2")
	} else if len(sl) == 2 {
		return [2]int(sl), nil
	}
	ans := [2]int{sl[0], sl[0]}
	for _, num := range sl {
		if num >= ans[0] {
			ans[1] = ans[0]
			ans[0] = num
		} else if num > ans[1] && num != ans[0] {
			ans[1] = num
		}
	}
	return ans, nil
}
