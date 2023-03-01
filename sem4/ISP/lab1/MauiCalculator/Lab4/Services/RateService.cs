
using MauiCalculator.Lab4.Entities;

namespace MauiCalculator.Lab4.Services;

public class RateService : IRateService
{
    private HttpClient _client;

    public RateService(HttpClient client)
    {
        _client = client;
    }

    public IEnumerable<Rate> GetRates(DateTime date)
    {
        throw new NotImplementedException();
    }
}
