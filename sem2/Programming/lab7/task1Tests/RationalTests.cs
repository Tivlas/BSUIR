using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace task1.Tests
{
    [TestClass()]
    public class RationalTests
    {
        [TestMethod()]
        public void ReduceFractionTest()
        {

            Rational[] expected = {
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 9),
                new Rational(5, 9),
                new Rational(16, 17),
                new Rational(7, 4)
            };

            Rational[] actual = {
                new Rational(4, 10),
                new Rational(16, 48),
                new Rational(9, 21),
                new Rational(16, 36),
                new Rational(25, 45),
                new Rational(32, 34),
                new Rational(49, 28)
            };


            for (int i = 0; i < expected.Length; i++)
            {
                actual[i].ReduceFraction();
            }

            for (int i = 0; i < actual.Length; i++)
            {
                Assert.AreEqual(expected[i].Numerator, actual[i].Numerator);
                Assert.AreEqual(expected[i].Denominator, actual[i].Denominator);
            }
        }

        [TestMethod()]
        public void operatorPlusTest()
        {
            Rational[] arr1 = {
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 9),
                new Rational(5, 9),
                new Rational(16, 17),
                new Rational(7, 4)
            };

            Rational[] arr2 = {
                new Rational(3, 10),
                new Rational(16, 189),
                new Rational(9, 1),
                new Rational(0, 36),
                new Rational(7, 45),
                new Rational(352, 1224),
                new Rational(69, 28)
            };

            Rational[] expected = {
                new Rational(7, 10),
                new Rational(79, 189),
                new Rational(66, 7),
                new Rational(4, 9),
                new Rational(32, 45),
                new Rational(188, 153),
                new Rational(59, 14)
            };

            Rational[] actual = {
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1)
            };

            for (int i = 0; i < arr1.Length; i++)
            {
                actual[i] = arr1[i] + arr2[i];
            }

            for (int i = 0; i < actual.Length; i++)
            {
                Assert.AreEqual(expected[i].Numerator, actual[i].Numerator);
                Assert.AreEqual(expected[i].Denominator, actual[i].Denominator);
            }
        }

        [TestMethod()]
        public void operatorMinusTest()
        {
            Rational[] arr1 = {
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 9),
                new Rational(5, 9),
                new Rational(16, 17),
                new Rational(7, 4)
            };

            Rational[] arr2 = {
                new Rational(3, 10),
                new Rational(16, 189),
                new Rational(9, 1),
                new Rational(0, 36),
                new Rational(7, 45),
                new Rational(352, 1224),
                new Rational(69, 28)
            };

            Rational[] expected = {
                new Rational(1, 10),
                new Rational(47, 189),
                new Rational(-60, 7),
                new Rational(4, 9),
                new Rational(2, 5),
                new Rational(100, 153),
                new Rational(-5, 7)
            };

            Rational[] actual = {
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1)
            };

            for (int i = 0; i < arr1.Length; i++)
            {
                actual[i] = arr1[i] - arr2[i];
            }

            for (int i = 0; i < actual.Length; i++)
            {
                Assert.AreEqual(expected[i].Numerator, actual[i].Numerator);
                Assert.AreEqual(expected[i].Denominator, actual[i].Denominator);
            }
        }

        [TestMethod()]
        public void operatorMultiplyTest()
        {
            Rational[] arr1 = {
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 9),
                new Rational(5, 9),
                new Rational(16, 17),
                new Rational(7, 4)
            };

            Rational[] arr2 = {
                new Rational(3, 10),
                new Rational(16, 189),
                new Rational(9, 1),
                new Rational(0, 36),
                new Rational(7, 45),
                new Rational(352, 1224),
                new Rational(69, 28)
            };

            Rational[] expected = {
                new Rational(3, 25),
                new Rational(16, 567),
                new Rational(27, 7),
                new Rational(0, 1),
                new Rational(7, 81),
                new Rational(704, 2601),
                new Rational(69, 16)
            };

            Rational[] actual = {
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1)
            };

            for (int i = 0; i < arr1.Length; i++)
            {
                actual[i] = arr1[i] * arr2[i];
            }

            for (int i = 0; i < actual.Length; i++)
            {
                Assert.AreEqual(expected[i].Numerator, actual[i].Numerator);
                Assert.AreEqual(expected[i].Denominator, actual[i].Denominator);
            }
        }

        [TestMethod()]
        public void operatorDivideTest()
        {
            Rational[] arr1 = {
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 9),
                new Rational(5, 9),
                new Rational(16, 17),
                new Rational(7, 4)
            };

            Rational[] arr2 = {
                new Rational(3, 10),
                new Rational(16, 189),
                new Rational(9, 1),
                new Rational(0, 36),
                new Rational(7, 45),
                new Rational(352, 1224),
                new Rational(69, 28)
            };

            Rational[] expected = {
                new Rational(4, 3),
                new Rational(63, 16),
                new Rational(1, 21),
                new Rational(0, 1),
                new Rational(25, 7),
                new Rational(36, 11),
                new Rational(49, 69)
            };

            Rational[] actual = {
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1)
            };

            for (int i = 0; i < arr1.Length; i++)
            {
                try
                {
                    actual[i] = arr1[i] / arr2[i];
                }
                catch (System.DivideByZeroException ex)
                {
                    System.Console.WriteLine(ex.Message);
                }
            }

            for (int i = 0; i < actual.Length; i++)
            {
                Assert.AreEqual(expected[i].Numerator, actual[i].Numerator);
                Assert.AreEqual(expected[i].Denominator, actual[i].Denominator);
            }
        }

        [TestMethod()]
        public void operatorIncrementTest()
        {
            Rational[] arr1 = {
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 9),
                new Rational(5, 9),
                new Rational(16, 17),
                new Rational(7, 4)
            };

            Rational[] expected1 = { // ++operand
                new Rational(7, 5),
                new Rational(4, 3),
                new Rational(10, 7),
                new Rational(13, 9),
                new Rational(14, 9),
                new Rational(33, 17),
                new Rational(11, 4)
            };

            Rational[] arr2 = {
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 9),
                new Rational(5, 9),
                new Rational(16, 17),
                new Rational(7, 4)
            };

            Rational[] expected2 = { // operand++
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 9),
                new Rational(5, 9),
                new Rational(16, 17),
                new Rational(7, 4)
            };

            Rational[] actual1 = { // ++operand
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1)
            };

            Rational[] actual2 = { // operand++
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1)
            };

            for (int i = 0; i < arr1.Length; i++)
            {
                actual1[i] = ++arr1[i];
            }

            for (int i = 0; i < arr2.Length; i++)
            {
                actual2[i] = arr2[i]++;
            }

            for (int i = 0; i < actual1.Length; i++)
            {
                Assert.AreEqual(expected1[i].Numerator, actual1[i].Numerator);
                Assert.AreEqual(expected1[i].Denominator, actual1[i].Denominator);
            }

            for (int i = 0; i < actual2.Length; i++)
            {
                Assert.AreEqual(expected2[i].Numerator, actual2[i].Numerator);
                Assert.AreEqual(expected2[i].Denominator, actual2[i].Denominator);
            }

            for (int i = 0; i < actual1.Length; i++) // после operand++ arr2[i] должен измениться
            {
                Assert.AreEqual(arr2[i].Numerator, arr1[i].Numerator);
                Assert.AreEqual(arr2[i].Denominator, arr1[i].Denominator);
            }
        }

        [TestMethod()]
        public void operatorDecrementTest()
        {
            Rational[] arr1 = {
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 9),
                new Rational(5, 9),
                new Rational(16, 17),
                new Rational(7, 4)
            };

            Rational[] expected1 = { // --operand
                new Rational(-3, 5),
                new Rational(-2, 3),
                new Rational(-4, 7),
                new Rational(-5, 9),
                new Rational(-4, 9),
                new Rational(-1, 17),
                new Rational(3, 4)
            };

            Rational[] arr2 = {
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 9),
                new Rational(5, 9),
                new Rational(16, 17),
                new Rational(7, 4)
            };

            Rational[] expected2 = { // operand--
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 9),
                new Rational(5, 9),
                new Rational(16, 17),
                new Rational(7, 4)
            };

            Rational[] actual1 = { // --operand
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1)
            };

            Rational[] actual2 = { // operand--
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1),
                new Rational(0, 1)
            };

            for (int i = 0; i < arr1.Length; i++)
            {
                actual1[i] = --arr1[i];
            }

            for (int i = 0; i < arr2.Length; i++)
            {
                actual2[i] = arr2[i]--;
            }

            for (int i = 0; i < actual1.Length; i++)
            {
                Assert.AreEqual(expected1[i].Numerator, actual1[i].Numerator);
                Assert.AreEqual(expected1[i].Denominator, actual1[i].Denominator);
            }

            for (int i = 0; i < actual2.Length; i++)
            {
                Assert.AreEqual(expected2[i].Numerator, actual2[i].Numerator);
                Assert.AreEqual(expected2[i].Denominator, actual2[i].Denominator);
            }

            for (int i = 0; i < actual1.Length; i++) // после operand-- arr2[i] должен измениться
            {
                Assert.AreEqual(arr2[i].Numerator, arr1[i].Numerator);
                Assert.AreEqual(arr2[i].Denominator, arr1[i].Denominator);
            }

        }

        [TestMethod()]
        public void operatorEqualTest()
        {
            Rational[] arr1 = {
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 9),
                new Rational(5, 9),
                new Rational(16, 17),
                new Rational(7, 4)
            };

            Rational[] arr2 = {
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 6),
                new Rational(5, 9),
                new Rational(16, 199),
                new Rational(7, 3)
            };

            bool[] expected = {
                true,
                true,
                true,
                false,
                true,
                false,
                false
            };

            bool[] actual = {
                false,
                false,
                false,
                false,
                false,
                false,
                false
            };

            for (int i = 0; i < arr1.Length; i++)
            {
                actual[i] = (arr1[i] == arr2[i]);
            }

            for (int i = 0; i < actual.Length; i++)
            {
                Assert.AreEqual(expected[i], actual[i]);
            }
        }

        [TestMethod()]
        public void operatorNotEqualTest()
        {
            Rational[] arr1 = {
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 9),
                new Rational(5, 9),
                new Rational(16, 17),
                new Rational(7, 4)
            };

            Rational[] arr2 = {
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 6),
                new Rational(5, 9),
                new Rational(16, 199),
                new Rational(7, 3)
            };

            bool[] expected = {
                false,
                false,
                false,
                true,
                false,
                true,
                true
            };

            bool[] actual = {
                false,
                false,
                false,
                false,
                false,
                false,
                false
            };

            for (int i = 0; i < arr1.Length; i++)
            {
                actual[i] = (arr1[i] != arr2[i]);
            }

            for (int i = 0; i < actual.Length; i++)
            {
                Assert.AreEqual(expected[i], actual[i]);
            }
        }

        [TestMethod()]
        public void operatorGreaterTest()
        {
            Rational[] arr1 = {
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 9),
                new Rational(5, 9),
                new Rational(16, 17),
                new Rational(7, 4)
            };

            Rational[] arr2 = {
                new Rational(2, 6),
                new Rational(6, 3),
                new Rational(3, 8),
                new Rational(4, 6),
                new Rational(5, 9),
                new Rational(16, 199),
                new Rational(7, 3)
            };

            bool[] expected = {
                true,
                false,
                true,
                false,
                false,
                true,
                false
            };

            bool[] actual = {
                false,
                false,
                false,
                false,
                false,
                false,
                false
            };

            for (int i = 0; i < arr1.Length; i++)
            {
                actual[i] = (arr1[i] > arr2[i]);
            }

            for (int i = 0; i < actual.Length; i++)
            {
                Assert.AreEqual(expected[i], actual[i]);
            }
        }

        [TestMethod()]
        public void operatorLessTest()
        {
            Rational[] arr1 = {
                new Rational(2, 5),
                new Rational(1, 3),
                new Rational(3, 7),
                new Rational(4, 9),
                new Rational(5, 9),
                new Rational(16, 17),
                new Rational(7, 4)
            };

            Rational[] arr2 = {
                new Rational(2, 6),
                new Rational(6, 3),
                new Rational(3, 8),
                new Rational(4, 6),
                new Rational(5, 9),
                new Rational(16, 199),
                new Rational(7, 3)
            };

            bool[] expected = {
                false,
                true,
                false,
                true,
                false,
                false,
                true
            };

            bool[] actual = {
                false,
                false,
                false,
                false,
                false,
                false,
                false
            };

            for (int i = 0; i < arr1.Length; i++)
            {
                actual[i] = (arr1[i] < arr2[i]);
            }

            for (int i = 0; i < actual.Length; i++)
            {
                Assert.AreEqual(expected[i], actual[i]);
            }
        }

        [TestMethod()]
        public void operatorConvertToIntTest()
        {
            Rational[] arr1 = {
                new Rational(2, 5),
                new Rational(3, 3),
                new Rational(3, 7),
                new Rational(19, 9),
                new Rational(5, 9),
                new Rational(34, 17),
                new Rational(7, 4)
            };

            int[] expected = {
                0,
                1,
                0,
                2,
                0,
                2,
                1
            };

            int[] actual = {
                0,
                0,
                0,
                0,
                0,
                0,
                0
            };

            for (int i = 0; i < arr1.Length; i++)
            {
                actual[i] = (int)arr1[i];
            }

            for (int i = 0; i < actual.Length; i++)
            {
                Assert.AreEqual(expected[i], actual[i]);
            }
        }

        [TestMethod()]
        public void operatorConvertToRationalTest()
        {
            int[] arr1 = {
                0,
                1,
                0,
                2,
                0,
                2,
                1
            };

            Rational[] expected = {
                new Rational(0, 1),
                new Rational(1, 1),
                new Rational(0, 1),
                new Rational(2, 1),
                new Rational(0, 1),
                new Rational(2, 1),
                new Rational(1, 1)
            };

            Rational[] actual = {
                new Rational(0, 0),
                new Rational(0, 0),
                new Rational(0, 0),
                new Rational(0, 0),
                new Rational(0, 0),
                new Rational(0, 0),
                new Rational(0, 0)
            };

            for (int i = 0; i < arr1.Length; i++)
            {
                actual[i] = (Rational)arr1[i];
            }

            for (int i = 0; i < actual.Length; i++)
            {
                Assert.AreEqual(expected[i].Numerator, actual[i].Numerator);
                Assert.AreEqual(expected[i].Denominator, actual[i].Denominator);
            }
        }

        [TestMethod()]
        public void ToStringTest()
        {
            Rational[] arr1 = {
                new Rational(2, 5),
                new Rational(3, 3),
                new Rational(3, 7),
                new Rational(19, 9),
                new Rational(5, 9),
                new Rational(34, 17),
                new Rational(7, 4)
            };

            string[] expected = {
                "2/5",
                "3/3",
                "3/7",
                "19/9",
                "5/9",
                "34/17",
                "7/4"
            };

            string[] actual = {
                "",
                "",
                "",
                "",
                "",
                "",
                ""
            };

            for (int i = 0; i < arr1.Length; i++)
            {
                actual[i] = arr1[i].ToString();
            }

            for (int i = 0; i < actual.Length; i++)
            {
                Assert.AreEqual(expected[i], actual[i]);
            }
        }
    }
}
