using MauiCalculator.Lab4.Entities;

namespace MauiCalculator.Lab4.Services
{
    public interface ICurrencyService
    {
        Task<IEnumerable<Currency>> GetCurrenciesAsync();
    }
}
