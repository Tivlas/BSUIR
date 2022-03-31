using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
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
            DateTime date1 = new(2012, 4, 14);
            DateTime date2 = new(2022, 4, 19);
            DateTime date3 = new(2022, 4, 3);
            DateTime now = DateTime.Now;

            int[] expected = {
                (int)(now.Date - date1.Date).TotalDays,
                (int)(now.Date - date2.Date).TotalDays,
                (int)(now.Date - date3.Date).TotalDays
            };
            int[] actual = {
                DateService.GetDaysSpan(date1.Day,date1.Month,date1.Year),
                DateService.GetDaysSpan(date2.Day,date2.Month,date2.Year),
                DateService.GetDaysSpan(date3.Day,date3.Month,date3.Year)
            };
            for (int i = 0; i < expected.Length; i++)
            {
                Assert.AreEqual(expected[i], actual[i]);
            }
        }
    }
}