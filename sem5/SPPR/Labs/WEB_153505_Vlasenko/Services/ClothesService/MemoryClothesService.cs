using Domain.Entities;
using Domain.Models;
using Microsoft.AspNetCore.Mvc;
using WEB_153505_Vlasenko.Services.ClothesCategoryService;

namespace WEB_153505_Vlasenko.Services.ClothesService;

public class MemoryClothesService : IClothesService
{
	List<ClothesCategory>? _categories;
	List<Clothes> _clothes;
	private readonly IConfiguration _configuration;

	private void SetupData()
	{
		var jacketsCategory = _categories?.Find(c => c.NormalizedName == "jackets");
		var shoesCategory = _categories?.Find(c => c.NormalizedName == "shoes");
		_clothes = new List<Clothes>()
		{
			new Clothes()
			{
				Id = 1,
				Name = "Куртка черная",
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
				Name = "Клоунская куртка",
				Description = "Если вы клоун, то это для вас",
				Price=1000000,
				ImagePath="images/ClownJacket.jpg",
				Mime = "image/jpeg",
				Category = jacketsCategory,
				CategoryId = jacketsCategory?.Id ?? 0
			},
			new Clothes()
			{
				Id = 3,
				Name = "Кожанка",
				Description = "Чудесная",
				Price=1000000,
				ImagePath="images/LeatherJacket.jpg",
				Mime = "image/jpeg",
				Category = jacketsCategory,
				CategoryId = jacketsCategory?.Id ?? 0
			},
			new Clothes()
			{
				Id = 4,
				Name = "Туфли",
				Description = "Не очень",
				Price=1000000,
				ImagePath="images/VelvetShoes.jpg",
				Mime = "image/jpeg",
				Category = shoesCategory,
				CategoryId = shoesCategory?.Id ?? 0
			},
			new Clothes()
			{
				Id = 5,
				Name = "Лапти",
				Description = "Для настоящих мужчин",
				Price=10315,
				ImagePath="images/Lapti.jpg",
				Mime = "image/jpeg",
				Category = shoesCategory,
				CategoryId = shoesCategory?.Id ?? 0
			},
			new Clothes()
			{
				Id = 6,
				Name = "Вроде сапоги",
				Description = "Крайне опасные сапоги",
				Price=1000000,
				ImagePath="images/SalamancaBoots.jpg",
				Mime = "image/jpeg",
				Category = shoesCategory,
				CategoryId = shoesCategory?.Id ?? 0
			},
			new Clothes()
			{
				Id = 7,
				Name = "Сапоги-змеи",
				Description = "Огонь!",
				Price=15090,
				ImagePath="images/SnakeShoes.jpg",
				Mime = "image/jpeg",
				Category = shoesCategory,
				CategoryId = shoesCategory?.Id ?? 0
			},
		};
	}

	public MemoryClothesService(IClothesCategoryService clothesCategoryService,
		[FromServices] IConfiguration configuration)
	{
		_categories = clothesCategoryService.GetClothesCategoryListAsync()?.Result?.Data;
		SetupData();
		_configuration = configuration;
	}

	public Task<ResponseData<ListModel<Clothes>>> GetClothesListAsync(string? categoryNormalizedName, int pageNo = 1)
	{
		var itemsPerPage = _configuration.GetValue<int>("ItemsPerPage");
		var itemsTemp = _clothes.
			Where(c => categoryNormalizedName == null || c.Category?.NormalizedName == categoryNormalizedName);
		int totalPages = (int)Math.Ceiling((double)itemsTemp.Count() / itemsPerPage);
		var items = itemsTemp
			.Skip((pageNo - 1) * itemsPerPage)
			.Take(itemsPerPage)
			.ToList();
		return Task.FromResult(new ResponseData<ListModel<Clothes>>()
		{
			//Success = !(items.Count() == 0),
			Success = true,
			Data = new ListModel<Clothes>()
			{
				Items = items,
				CurrentPage = pageNo,
				TotalPages = totalPages
			},
			ErrorMessage = !(items.Count() == 0) ? "" : "Ошибка :("
		});
	}
	public Task<ResponseData<Clothes>> GetClothesByIdAsync(int id) => throw new NotImplementedException();
	public Task UpdateClothesAsync(int id, Clothes product, IFormFile? formFile) => throw new NotImplementedException();
	public Task DeleteClothesAsync(int id) => throw new NotImplementedException();
	public Task<ResponseData<Clothes>> CreateClothesAsync(Clothes product, IFormFile? formFile) => throw new NotImplementedException();
}
