using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace dateServiceNamespace.Tests
{
    [TestClass()]
    public class DateServiceTests
    {
        [TestMethod()]
        public void GetDayTest()
        {
            int[] expected = { 1, 2, 3, 4, 5, 6, 0 };
            int[] actual = {
                DateService.GetDay("04.04.2022"),
                DateService.GetDay("07.06.2022"),
                DateService.GetDay("04.08.2021"),
                DateService.GetDay("19.08.2021"),
                DateService.GetDay("14.04.2017"),
                DateService.GetDay("17.06.2017"),
                DateService.GetDay("21.02.2021")
            };
            for (int i = 0; i < expected.Length; i++)
            {
                Assert.AreEqual(expected[i], actual[i]);
            }
        }

        [TestMethod()]
        public void GetDaysSpan()
        {
            int[] expected = { 14, 29, 3 };
            int[] actual = {
                DateService.GetDaysSpan(14,4,2022),
                DateService.GetDaysSpan(29, 4, 2022),
                DateService.GetDaysSpan(3,4,2022)
            };
            for (int i = 0; i < expected.Length; i++)
            {
                Assert.AreEqual(expected[i], actual[i]);
            }
        }
    }
}