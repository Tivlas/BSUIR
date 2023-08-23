using Domain.Entities;
using Domain.Models;
using WEB_153505_Vlasenko.Services.ClothesCategoryService;

namespace WEB_153505_Vlasenko.Services.ClothesService;

public class MemoryClothesService : IClothesService
{
	List<ClothesCategory>? _categories;
	List<Clothes> _clothes;

	private void SetupData()
	{
		var jacketsCategory = _categories?.Find(c => c.NormalizedName == "jackets");
		var shoesCategory = _categories?.Find(c => c.NormalizedName == "shoes");
		_clothes = new List<Clothes>()
		{
			new Clothes()
			{
				Id = 1,
				Name = "Куртка",
				Description = "Балдежная черная куртка",
				Price=100.99M,
				ImagePath="images/BlackJacket.jpg",
				Mime = "image/jpeg",
				Category = jacketsCategory,
				CategoryId = jacketsCategory?.Id ?? 0
			},
			new Clothes()
			{
				Id = 2,
				Name = "Туфли",
				Description = "Бархатные",
				Price=1000000,
				ImagePath="images/VelvetShoes.jpg",
				Mime = "image/jpeg",
				Category = shoesCategory,
				CategoryId = shoesCategory?.Id ?? 0
			},
		};
	}

	public MemoryClothesService(IClothesCategoryService clothesCategoryService)
	{
		_categories = clothesCategoryService.GetClothesCategoryListAsync()?.Result?.Data;
		SetupData();
	}

	public Task<ResponseData<ListModel<Clothes>>> GetClothesListAsync(string? categoryNormalizedName, int pageNo = 1)
	{
		var items = _clothes
			.Where(c => categoryNormalizedName == null || c.Category?.NormalizedName == categoryNormalizedName)
			.ToList();
		return Task.FromResult(new ResponseData<ListModel<Clothes>>()
		{
			Success = !(items.Count() == 0),
			Data = new ListModel<Clothes>()
			{
				Items = items,
				CurrentPage = pageNo,
			},
			ErrorMessage = !(items.Count() == 0) ? "" : "Нет одежды такой категории"
		});
	}
	public Task<ResponseData<Clothes>> GetClothesByIdAsync(int id) => throw new NotImplementedException();
	public Task UpdateClothesAsync(int id, Clothes product, IFormFile? formFile) => throw new NotImplementedException();
	public Task DeleteClothesAsync(int id) => throw new NotImplementedException();
	public Task<ResponseData<Clothes>> CreateClothesAsync(Clothes product, IFormFile? formFile) => throw new NotImplementedException();
}
