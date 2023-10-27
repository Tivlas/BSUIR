using Domain.Entities;

namespace BlazorWasm.Services;

public interface IDataService
{
	event Action DataChanged;
	List<ClothesCategory>? Categories { get; set; }
	List<Clothes>? ClothesList { get; set; }
	bool Success { get; set; }
	string? ErrorMessage { get; set; }
	int TotalPages { get; set; }
	int CurrentPage { get; set; }

	public Task GetClothesListAsync(string? categoryNormalizedName, int pageNo = 1);

	public Task<Clothes?> GetClothesByIdAsync(int id);

	public Task GetCategoryListAsync();
}
