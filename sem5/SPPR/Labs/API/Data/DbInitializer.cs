using Domain.Entities;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;

namespace API.Data;

public class DbInitializer
{
	public static async Task SeedData(WebApplication app)
	{
		var categories = new List<ClothesCategory>
		{
			new ClothesCategory { Name="Обувь", NormalizedName="shoes"},
			new ClothesCategory { Name="Куртки", NormalizedName="jackets"},
		};
		var jacketsCategory = categories?.Find(c => c.NormalizedName == "jackets");
		var shoesCategory = categories?.Find(c => c.NormalizedName == "shoes");
		var clothes = new List<Clothes>()
		{
			new Clothes()
			{
				Name = "Куртка черная",
				Description = "Балдежная черная куртка",
				Price=100.99M,
				ImagePath="BlackJacket.jpg",
				Mime = "image/jpeg",
				Category = jacketsCategory,
				//CategoryId = jacketsCategory?.Id ?? 0
			},
			new Clothes()
			{
				Name = "Клоунская куртка",
				Description = "Если вы клоун, то это для вас",
				Price=1000000,
				ImagePath="ClownJacket.jpg",
				Mime = "image/jpeg",
				Category = jacketsCategory,
				//CategoryId = jacketsCategory?.Id ?? 0
			},
			new Clothes()
			{
				Name = "Кожанка",
				Description = "Чудесная",
				Price=1000000,
				ImagePath="LeatherJacket.jpg",
				Mime = "image/jpeg",
				Category = jacketsCategory,
				//CategoryId = jacketsCategory?.Id ?? 0
			},
			new Clothes()
			{
				Name = "Туфли",
				Description = "Не очень",
				Price=1000000,
				ImagePath="VelvetShoes.jpg",
				Mime = "image/jpeg",
				Category = shoesCategory,
				//CategoryId = shoesCategory?.Id ?? 0
			},
			new Clothes()
			{
				Name = "Лапти",
				Description = "Для настоящих мужчин",
				Price=10315,
				ImagePath="Lapti.jpg",
				Mime = "image/jpeg",
				Category = shoesCategory,
				//CategoryId = shoesCategory?.Id ?? 0
			},
			new Clothes()
			{
				Name = "Вроде сапоги",
				Description = "Крайне опасные сапоги",
				Price=1000000,
				ImagePath="SalamancaBoots.jpg",
				Mime = "image/jpeg",
				Category = shoesCategory,
				//CategoryId = shoesCategory?.Id ?? 0
			},
			new Clothes()
			{
				Name = "Сапоги-змеи",
				Description = "Огонь!",
				Price=15090,
				ImagePath="SnakeShoes.jpg",
				Mime = "image/jpeg",
				Category = shoesCategory,
				//CategoryId = shoesCategory?.Id ?? 0
			},
		};

		using var scope = app.Services.CreateScope();

		var context = scope.ServiceProvider.GetRequiredService<AppDbContext>();

		if (context.Database.GetPendingMigrations().Any())
		{
			await context.Database.MigrateAsync();
		}

		if (!context.ClothesCategory.Any())
		{
			await context.ClothesCategory.AddRangeAsync(categories!);
			await context.SaveChangesAsync();
		}

		var imageBaseUrl = app.Configuration.GetValue<string>("ImageUrl");
		if (!context.Clothes.Any())
		{

			foreach (var cloth in clothes)
			{
				cloth.ImagePath = $"{imageBaseUrl}/{cloth.ImagePath}";
			}
			await context.Clothes.AddRangeAsync(clothes);
			await context.SaveChangesAsync();
		}
	}
}
