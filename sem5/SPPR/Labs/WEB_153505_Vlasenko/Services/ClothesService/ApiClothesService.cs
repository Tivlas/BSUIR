using System.Text;
using System.Text.Json;
using Domain.Entities;
using Domain.Models;
using WEB_153505_Vlasenko.Services.ClothesCategoryService;

namespace WEB_153505_Vlasenko.Services.ClothesService;

public class ApiClothesService : IClothesService
{
	private readonly HttpClient _httpClient;
	private readonly ILogger<ApiClothesService> _logger;
	private readonly JsonSerializerOptions _jsonSerializerOptions;
	private readonly int _pageSize;

	public ApiClothesService(HttpClient httpClient, IConfiguration configuration, ILogger<ApiClothesService> logger)
	{
		_httpClient = httpClient;
		_logger = logger;
		_jsonSerializerOptions = new JsonSerializerOptions()
		{
			PropertyNamingPolicy = JsonNamingPolicy.CamelCase
		};
		_pageSize = configuration.GetValue<int>("ItemsPerPage");
	}

	public async Task<ResponseData<ListModel<Clothes>>> GetClothesListAsync(string? categoryNormalizedName, int pageNo = 1)
	{
		var urlString = new StringBuilder($"{_httpClient.BaseAddress!.AbsoluteUri}clothes/");
		if (categoryNormalizedName != null)
		{
			urlString.Append($"{categoryNormalizedName}/");
		};
		if (pageNo > 1)
		{
			urlString.Append($"{pageNo}");
		};
		if (!_pageSize.Equals("3"))
		{
			urlString.Append(QueryString.Create("pageSize", _pageSize.ToString()));
		}

		var response = await _httpClient.GetAsync(new Uri(urlString.ToString()));
		if (response.IsSuccessStatusCode)
		{
			try
			{
				return await response.Content.ReadFromJsonAsync<ResponseData<ListModel<Clothes>>>(_jsonSerializerOptions);
			}
			catch (JsonException ex)
			{
				_logger.LogError($"-----> Ошибка: {ex.Message}");
				return new ResponseData<ListModel<Clothes>>
				{
					Success = false,
					ErrorMessage = $"Ошибка: {ex.Message}"
				};
			}
		}
		_logger.LogError($"-----> Данные не получены от сервера. Error:{response.StatusCode}");
		return new ResponseData<ListModel<Clothes>>()
		{
			Success = false,
			ErrorMessage = $"Данные не получены от сервера. Error:{response.StatusCode}"
		};
	}

	public Task<ResponseData<Clothes>> GetClothesByIdAsync(int id) => throw new NotImplementedException();
	public Task UpdateClothesAsync(int id, Clothes product, IFormFile? formFile) => throw new NotImplementedException();
	public Task DeleteClothesAsync(int id) => throw new NotImplementedException();
	public Task<ResponseData<Clothes>> CreateClothesAsync(Clothes product, IFormFile? formFile) => throw new NotImplementedException();
}
