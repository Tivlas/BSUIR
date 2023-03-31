using MauiCalculator.Lab4.Entities;

namespace MauiCalculator.Lab4.Services
{
    public interface IRateService
    {
        Task<Rate> GetRate(DateTime date, Currency currency);
    }
}
