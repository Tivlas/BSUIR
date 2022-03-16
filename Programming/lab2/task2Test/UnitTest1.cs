using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace task2Test
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void Yes()
        {
            double x = 4;
            double y = 5;
            int expected = 1;
            Point point = new Point();
            int actual = point.BelongToArea(x,y);
            Assert.AreEqual(expected, actual);
          
        }

        [TestMethod]
        public void OnTheBorder()
        {
            double x = 7;
            double y = 0;
            int expected = 2;
            Point point = new Point();
            int actual = point.BelongToArea(x, y);
            Assert.AreEqual(expected, actual);

        }

        [TestMethod]
        public void No()
        {
            double x = 1.3;
            double y = 2;
            int expected = 3;
            Point point = new Point();
            int actual = point.BelongToArea(x, y);
            Assert.AreEqual(expected, actual);
        }
    }
}