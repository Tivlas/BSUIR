using System.Data.Common;
using API.Data;
using API.Services;
using Domain.Entities;
using Domain.Models;
using Microsoft.Data.Sqlite;
using Microsoft.EntityFrameworkCore;

namespace Tests;
public class ApiProjectClothesServiceTest : IDisposable
{
	public void Dispose() => _connection.Dispose();

	private readonly DbConnection _connection;
	private readonly DbContextOptions<AppDbContext> _contextOptions;

	public ApiProjectClothesServiceTest()
	{
		_connection = new SqliteConnection("Filename=:memory:");
		_connection.Open();

		_contextOptions = new DbContextOptionsBuilder<AppDbContext>()
			.UseSqlite(_connection)
			.Options;

		using var context = new AppDbContext(_contextOptions);
		context.Database.EnsureCreated();
		context.ClothesCategory.AddRange(
			new ClothesCategory() { Id = 1, Name = "Обувь", NormalizedName = "shoes" },
			new ClothesCategory() { Id = 2, Name = "Куртки", NormalizedName = "jackets" });
		context.Clothes.AddRange(
			new Clothes() { Id = 1, Price = 10.2M, Name = "Jacket1", CategoryId = 2 },
			new Clothes() { Id = 2, Price = 10.2M, Name = "Jacket2", CategoryId = 2 },
			new Clothes() { Id = 3, Price = 10.2M, Name = "Jacket3", CategoryId = 2 },
			new Clothes() { Id = 4, Price = 10.2M, Name = "Jacket4", CategoryId = 2 },
			new Clothes() { Id = 5, Price = 10.2M, Name = "Shoes1", CategoryId = 1 },
			new Clothes() { Id = 6, Price = 10.2M, Name = "Shoes2", CategoryId = 1 },
			new Clothes() { Id = 7, Price = 10.2M, Name = "Shoes3", CategoryId = 1 });
		context.SaveChanges();
	}

	private AppDbContext CreateContext() => new(_contextOptions);

	[Fact]
	public void GetClothesListAsync_ReturnsFirstPageWithThreeItems_WhenDefaultParametersPassed()
	{
		// Arrange
		using var context = CreateContext();
		ClothesService service = new(context, null!, null!);

		// Act
		var result = service.GetClothesListAsync(null).Result;

		// Assert
		Assert.IsType<ResponseData<ListModel<Clothes>>>(result);
		Assert.True(result.Success);
		Assert.Equal(1, result.Data!.CurrentPage);
		Assert.Equal(3, result.Data.Items.Count);
		Assert.Equal(3, result.Data.TotalPages);
		Assert.Equal(context.Clothes.First(), result.Data.Items[0]);
	}

	[Fact]
	public void GetClothesListAsync_ReturnsSecondPageWithSecondThreeItems_WhenPageParameterEquals2()
	{
		// Arrange
		using var context = CreateContext();
		ClothesService service = new(context, null!, null!);

		// Act
		var result = service.GetClothesListAsync(null, 2).Result;

		// Assert
		Assert.IsType<ResponseData<ListModel<Clothes>>>(result);
		Assert.True(result.Success);
		Assert.Equal(2, result.Data!.CurrentPage);
		Assert.Equal(3, result.Data.Items.Count);
		Assert.Equal(3, result.Data.TotalPages);
		Assert.Equal(context.Clothes.Skip(3).First(), result.Data.Items.First());
	}

	[Fact]
	public void GetClothesListAsync_ReturnsValidClothesByCategory_WhenCategoryParameterPassed()
	{
		// Arrange
		using var context = CreateContext();
		ClothesService service = new(context, null!, null!);

		// Act
		var result = service.GetClothesListAsync("jackets").Result;

		// Assert
		Assert.IsType<ResponseData<ListModel<Clothes>>>(result);
		Assert.True(result.Success);
		Assert.Equal(1, result.Data!.CurrentPage);
		Assert.Equal(3, result.Data.Items.Count);
		Assert.Equal(2, result.Data.TotalPages);
		Assert.DoesNotContain(result.Data.Items, x => x.CategoryId != 2);
	}

	[Fact]
	public void GetClothesListAsync_ReturnsSuccessFalse_WhenPageNumberParameterIsGreaterThanTotalPages()
	{
		// Arrange
		using var context = CreateContext();
		ClothesService service = new(context, null!, null!);

		// Act
		var result = service.GetClothesListAsync(null, 1000).Result;

		// Assert
		Assert.IsType<ResponseData<ListModel<Clothes>>>(result);
		Assert.False(result.Success);
	}

	[Fact]
	public void GetClothesListAsync_DoesNotAllowToSetPageSizeGreaterThanMaxPageSize_WhenGreaterPageSizePassed()
	{
		// Arrange
		using var context = CreateContext();
		ClothesService service = new(context, null!, null!);

		// Act
		var result = service.GetClothesListAsync(null!, 1, service.MaxPageSize + 1).Result;

		// Assert
		Assert.True(result.Success);
		Assert.True(result.Data!.Items.Count <= service.MaxPageSize);
	}
}
