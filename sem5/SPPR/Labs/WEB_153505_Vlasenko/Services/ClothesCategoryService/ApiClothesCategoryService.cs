using System.Text;
using System.Text.Json;
using Domain.Entities;
using Domain.Models;

namespace WEB_153505_Vlasenko.Services.ClothesCategoryService;

public class ApiClothesCategoryService : IClothesCategoryService
{
    private readonly HttpClient _httpClient;
    private readonly ILogger<ApiClothesCategoryService> _logger;
    private readonly JsonSerializerOptions _jsonSerializerOptions;

    public ApiClothesCategoryService(HttpClient httpClient, ILogger<ApiClothesCategoryService> logger)
    {
        _httpClient = httpClient;
        _logger = logger;
        _jsonSerializerOptions = new JsonSerializerOptions()
        {
            PropertyNamingPolicy = JsonNamingPolicy.CamelCase
        };
    }

    public async Task<ResponseData<List<ClothesCategory>>> GetClothesCategoryListAsync()
    {
        var urlString = new StringBuilder($"{_httpClient.BaseAddress?.AbsoluteUri}clothesCategories/");
        var response = await _httpClient.GetAsync(new Uri(urlString.ToString()));
        if (response.IsSuccessStatusCode)
        {
            try
            {
                return await response.Content.ReadFromJsonAsync<ResponseData<List<ClothesCategory>>>(_jsonSerializerOptions);
            }
            catch (JsonException ex)
            {
                _logger.LogError($"-----> Ошибка: {ex.Message}");
                return new ResponseData<List<ClothesCategory>>
                {
                    Success = false,
                    ErrorMessage = $"Ошибка: {ex.Message}"
                };

            }
        }
        _logger.LogError($"-----> Данные не получены от сервера. Error:{response.StatusCode}");
        return new ResponseData<List<ClothesCategory>>()
        {
            Success = false,
            ErrorMessage = $"Данные не получены от сервера. Error:{response.StatusCode}"
        };
    }
}
