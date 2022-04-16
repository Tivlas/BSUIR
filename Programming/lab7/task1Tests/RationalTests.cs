using task1;
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
                new Rational(1, 7),
                new Rational(0, 0),
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
                actual[i] = arr1[i] / arr2[i];
            }

            for (int i = 0; i < actual.Length; i++)
            {
                Assert.AreEqual(expected[i].Numerator, actual[i].Numerator);
                Assert.AreEqual(expected[i].Denominator, actual[i].Denominator);
            }
        }
    }
}
