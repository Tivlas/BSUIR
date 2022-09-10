using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
namespace numbers.Tests
{
    [TestClass()]
    public class NumbersTests
    {
        [TestMethod()]
        public void maxTest()
        {
            double[] actual = {
                Numbers.max(4, 5),
                Numbers.max(-3, 5),
                Numbers.max(23.333, 23.332),
                Numbers.max(-323.433, -323.432),
                Numbers.max(123, 123.001),
                Numbers.max(4.5, 4.3)
            };

            double[] expected = { 5, 5, 23.333, -323.432, 123.001, 4.5 };
            for (int i = 0; i < actual.Length; i++)
            {
                Assert.AreEqual(expected[i], actual[i]);
            }
        }

        [TestMethod()]
        public void calcTest()
        {
            double[] actual = {
                Numbers.calc(2, 1),
                Numbers.calc(33, 256),
                Numbers.calc(27, 3.3),
                Numbers.calc(-3, -5),
                Numbers.calc(0, 0),
                Math.Round(Numbers.calc(2.33, 2.56),2)
            };

            double[] expected = { -12, -198, -162, 2, 0, -13.98 };
            for (int j = 0; j < actual.Length; j++)
            {
                Assert.AreEqual(expected[j], actual[j]);
            }
        }
    }
}