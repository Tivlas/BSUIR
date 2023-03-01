using MauiCalculator.Lab4.Entities;

namespace MauiCalculator.Lab4.Services
{
    public interface IRateService
    {
        IEnumerable<Rate> GetRates(DateTime date);
    }
}
