using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace task1Test
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void EndsWithSeven77()
        {
            int x = 77;
            bool expected_1 = true;
            Number num = new Number();
            bool actual_1 = num.EndsWith7(x);
            Assert.AreEqual(expected_1, actual_1);
        }

        [TestMethod]
        public void EndsWithSeven183()
        {
            int y = 183;
            bool expected_1 = false;
            Number num = new Number();
            bool actual_1 = num.EndsWith7(y);
            Assert.AreEqual(expected_1, actual_1);
        }
    }
}