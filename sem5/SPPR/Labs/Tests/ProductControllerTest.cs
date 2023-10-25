using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Domain.Entities;
using Domain.Models;
using Microsoft.AspNetCore.Mvc;
using Moq;
using WEB_153505_Vlasenko.Controllers;
using WEB_153505_Vlasenko.Services.ClothesService;
using WEB_153505_Vlasenko.Services.ClothesCategoryService;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.Primitives;

namespace Tests;

class ClothesCategoryComparer : IEqualityComparer<ClothesCategory>
{
	public bool Equals(ClothesCategory? x, ClothesCategory? y)
	{
		if (ReferenceEquals(x, y))
			return true;

		if (ReferenceEquals(x, null) || ReferenceEquals(y, null))
			return false;

		return x.Id == y.Id && x.Name == y.Name && x.NormalizedName == y.NormalizedName;
	}

	public int GetHashCode(ClothesCategory obj)
	{
		int hash = 17;
		hash = hash * 23 + obj.Id.GetHashCode();
		hash = hash * 23 + obj.Name.GetHashCode();
		hash = hash * 23 + obj.NormalizedName.GetHashCode();
		return hash;
	}
}

public class ProductControllerTest
{
	private List<ClothesCategory> GetSampleCategories()
	{
		return new List<ClothesCategory>() {
				new ClothesCategory() { Id = 1, Name="Обувь", NormalizedName="shoes"},
				new ClothesCategory() { Id = 2, Name="Куртки", NormalizedName="jackets"}
			};
	}

	private List<Clothes> GetSampleClothes()
	{
		return new List<Clothes>()
				{
					new Clothes() { Id = 1, Price=10.2M, Name="Jacket1", CategoryId=2},
					new Clothes() { Id = 1, Price=10.2M, Name="Shoes1", CategoryId=1},
				};
	}

	[Fact]
	public void Index_ReturnsViewWithListOfClothesModel_WhenNotAjaxQuery()
	{
		//Arrange
		Mock<IClothesCategoryService> categories_moq = new();
		categories_moq.Setup(m => m.GetClothesCategoryListAsync()).ReturnsAsync(new ResponseData<List<ClothesCategory>>()
		{
			Data = GetSampleCategories()
		});

		Mock<IClothesService> clothes_moq = new();
		clothes_moq.Setup(m => m.GetClothesListAsync(null, 1)).ReturnsAsync(new ResponseData<ListModel<Clothes>>()
		{
			Success = true,
			ErrorMessage = null,
			Data = new ListModel<Clothes>()
			{
				Items = GetSampleClothes()
			}
		});

		var header = new Dictionary<string, StringValues>();
		var controllerContext = new ControllerContext();
		var moqHttpContext = new Mock<HttpContext>();
		moqHttpContext.Setup(c => c.Request.Headers).Returns(new HeaderDictionary(header));
		controllerContext.HttpContext = moqHttpContext.Object;

		//Act
		var controller = new ProductController(clothes_moq.Object, categories_moq.Object) { ControllerContext = controllerContext };
		var result = controller.Index(null).Result;
		//Assert
		Assert.NotNull(result);
		var viewResult = Assert.IsType<ViewResult>(result);
		Assert.IsType<List<Clothes>>(viewResult.Model);
	}

	[Fact]
	public void Index_ReturnsPartialView_WhenAjaxQuery()
	{
		//Arrange
		Mock<IClothesCategoryService> categories_moq = new();
		categories_moq.Setup(m => m.GetClothesCategoryListAsync()).ReturnsAsync(new ResponseData<List<ClothesCategory>>()
		{
			Data = new List<ClothesCategory>() {
				new ClothesCategory() { Id = 1, Name="shoes", NormalizedName="Обувь"},
				new ClothesCategory() { Id = 2, Name="jackets", NormalizedName="Куртки"}
			}
		});

		Mock<IClothesService> clothes_moq = new();
		clothes_moq.Setup(m => m.GetClothesListAsync(null, 1)).ReturnsAsync(new ResponseData<ListModel<Clothes>>()
		{
			Success = true,
			ErrorMessage = null,
			Data = new ListModel<Clothes>()
			{
				Items = new List<Clothes>()
				{
					new Clothes() { Id = 1, Price=10.2M, Name="Jacket1", CategoryId=2},
					new Clothes() { Id = 1, Price=10.2M, Name="Shoes1", CategoryId=1},
				}
			}
		});

		var header = new Dictionary<string, StringValues>()
		{
			["X-Requested-With"] = "XMLHttpRequest"
		};
		var controllerContext = new ControllerContext();
		var moqHttpContext = new Mock<HttpContext>();
		moqHttpContext.Setup(c => c.Request.Headers).Returns(new HeaderDictionary(header));
		controllerContext.HttpContext = moqHttpContext.Object;

		//Act
		var controller = new ProductController(clothes_moq.Object, categories_moq.Object) { ControllerContext = controllerContext };
		var result = controller.Index(null).Result;
		//Assert
		Assert.NotNull(result);
		var viewResult = Assert.IsType<PartialViewResult>(result);
	}

	[Fact]
	public void Index_ReturnsError404_WhenUnsuccessfullyReceivedCategories()
	{
		//Arrange
		Mock<IClothesCategoryService> categories_moq = new();
		categories_moq.Setup(m => m.GetClothesCategoryListAsync()).ReturnsAsync(new ResponseData<List<ClothesCategory>>()
		{
			Success = false
		});

		Mock<IClothesService> clothes_moq = new();
		clothes_moq.Setup(m => m.GetClothesListAsync(null, 1)).ReturnsAsync(new ResponseData<ListModel<Clothes>>()
		{
			Success = true
		});

		var header = new Dictionary<string, StringValues>();
		var controllerContext = new ControllerContext();
		var moqHttpContext = new Mock<HttpContext>();
		moqHttpContext.Setup(c => c.Request.Headers).Returns(new HeaderDictionary(header));
		controllerContext.HttpContext = moqHttpContext.Object;

		//Act
		var controller = new ProductController(clothes_moq.Object, categories_moq.Object) { ControllerContext = controllerContext };
		var result = controller.Index(null).Result;
		//Assert
		Assert.NotNull(result);
		var viewResult = Assert.IsType<NotFoundObjectResult>(result);
		Assert.Equal(StatusCodes.Status404NotFound, viewResult.StatusCode);
	}

	[Fact]
	public void Index_ReturnsError404_WhenUnsuccessfullyReceivedClothes()
	{
		//Arrange
		Mock<IClothesCategoryService> categories_moq = new();
		categories_moq.Setup(m => m.GetClothesCategoryListAsync()).ReturnsAsync(new ResponseData<List<ClothesCategory>>()
		{
			Success = true
		});

		Mock<IClothesService> clothes_moq = new();
		clothes_moq.Setup(m => m.GetClothesListAsync(null, 1)).ReturnsAsync(new ResponseData<ListModel<Clothes>>()
		{
			Success = false
		});

		var header = new Dictionary<string, StringValues>();
		var controllerContext = new ControllerContext();
		var moqHttpContext = new Mock<HttpContext>();
		moqHttpContext.Setup(c => c.Request.Headers).Returns(new HeaderDictionary(header));
		controllerContext.HttpContext = moqHttpContext.Object;

		//Act
		var controller = new ProductController(clothes_moq.Object, categories_moq.Object) { ControllerContext = controllerContext };
		var result = controller.Index(null).Result;
		//Assert
		Assert.NotNull(result);
		var viewResult = Assert.IsType<NotFoundObjectResult>(result);
		Assert.Equal(StatusCodes.Status404NotFound, viewResult.StatusCode);
	}

	[Fact]
	public void Index_ViewDataContainsCategories_WhenSuccessfullyReceivedData()
	{
		//Arrange
		Mock<IClothesCategoryService> categories_moq = new();
		categories_moq.Setup(m => m.GetClothesCategoryListAsync()).ReturnsAsync(new ResponseData<List<ClothesCategory>>()
		{
			Data = GetSampleCategories()
		});

		Mock<IClothesService> clothes_moq = new();
		clothes_moq.Setup(m => m.GetClothesListAsync(null, 1)).ReturnsAsync(new ResponseData<ListModel<Clothes>>()
		{
			Success = true,
			ErrorMessage = null,
			Data = new ListModel<Clothes>()
			{
				Items = GetSampleClothes()
			}
		});

		var header = new Dictionary<string, StringValues>();
		var controllerContext = new ControllerContext();
		var moqHttpContext = new Mock<HttpContext>();
		moqHttpContext.Setup(c => c.Request.Headers).Returns(new HeaderDictionary(header));
		controllerContext.HttpContext = moqHttpContext.Object;


		//Act
		var controller = new ProductController(clothes_moq.Object, categories_moq.Object) { ControllerContext = controllerContext };
		var result = controller.Index(null).Result;
		//Assert
		Assert.NotNull(result);
		var viewResult = Assert.IsType<ViewResult>(result);
		Assert.True(viewResult.ViewData.ContainsKey("allCategories"));

		Assert.Equal(GetSampleCategories(), viewResult.ViewData["allCategories"] as IEnumerable<ClothesCategory>, new ClothesCategoryComparer());
	}

	[Fact]
	public void Index_ViewDataContainsValidCurrentCategoryValue_WhenCategoryParameterIsNotNull()
	{
		//Arrange
		Mock<IClothesCategoryService> categories_moq = new();
		categories_moq.Setup(m => m.GetClothesCategoryListAsync()).ReturnsAsync(new ResponseData<List<ClothesCategory>>()
		{
			Data = GetSampleCategories()
		});

		Mock<IClothesService> clothes_moq = new();
		clothes_moq.Setup(m => m.GetClothesListAsync("shoes", 1)).ReturnsAsync(new ResponseData<ListModel<Clothes>>()
		{
			Data = new ListModel<Clothes>()
			{
				Items = GetSampleClothes()
			}
		});

		var header = new Dictionary<string, StringValues>();
		var controllerContext = new ControllerContext();
		var moqHttpContext = new Mock<HttpContext>();
		moqHttpContext.Setup(c => c.Request.Headers).Returns(new HeaderDictionary(header));
		controllerContext.HttpContext = moqHttpContext.Object;

		//Act
		var controller = new ProductController(clothes_moq.Object, categories_moq.Object) { ControllerContext = controllerContext };
		var result = controller.Index("shoes", 1).Result;
		//Assert
		Assert.NotNull(result);
		var viewResult = Assert.IsType<ViewResult>(result);
		Assert.True(viewResult.ViewData.ContainsKey("currentCategory"));
		Assert.Equal("Обувь", viewResult.ViewData["currentCategory"] as string);
	}

	[Fact]
	public void Index_ViewDataContainsValidCurrentCategoryValue_WhenCategoryParameterIsNull()
	{
		//Arrange
		Mock<IClothesCategoryService> categories_moq = new();
		categories_moq.Setup(m => m.GetClothesCategoryListAsync()).ReturnsAsync(new ResponseData<List<ClothesCategory>>()
		{
			Data = GetSampleCategories()
		});

		Mock<IClothesService> clothes_moq = new();
		clothes_moq.Setup(m => m.GetClothesListAsync(null, 1)).ReturnsAsync(new ResponseData<ListModel<Clothes>>()
		{
			Data = new ListModel<Clothes>()
			{
				Items = GetSampleClothes()
			}
		});

		var header = new Dictionary<string, StringValues>();
		var controllerContext = new ControllerContext();
		var moqHttpContext = new Mock<HttpContext>();
		moqHttpContext.Setup(c => c.Request.Headers).Returns(new HeaderDictionary(header));
		controllerContext.HttpContext = moqHttpContext.Object;

		//Act
		var controller = new ProductController(clothes_moq.Object, categories_moq.Object) { ControllerContext = controllerContext };
		var result = controller.Index(null).Result;
		//Assert
		Assert.NotNull(result);
		var viewResult = Assert.IsType<ViewResult>(result);
		Assert.True(viewResult.ViewData.ContainsKey("currentCategory"));
		Assert.Equal("Все", viewResult.ViewData["currentCategory"] as string);
	}
}
