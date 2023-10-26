using API.Data;
using Domain.Entities;
using Domain.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace API.Services;

public class ClothesService : IClothesService
{
	private readonly AppDbContext _dbContext;
	private readonly IWebHostEnvironment _webHostEnvironment;
	private readonly IHttpContextAccessor _httpContextAccessor;
	private readonly int _maxPageSize = 20;

	public ClothesService(AppDbContext dbContext,
		IWebHostEnvironment webHostEnvironment, IHttpContextAccessor httpContextAccessor)
	{
		_dbContext = dbContext;
		_webHostEnvironment = webHostEnvironment;
		_httpContextAccessor = httpContextAccessor;
	}

	public async Task<ResponseData<ListModel<Clothes>>> GetClothesListAsync(string? categoryNormalizedName, int pageNo = 1, int pageSize = 3)
	{
		if (pageSize > _maxPageSize)
		{
			pageSize = _maxPageSize;
		}

		var query = _dbContext.Clothes.AsQueryable();
		var dataList = new ListModel<Clothes>();
		query = query.Where(d => categoryNormalizedName == null
		|| d.Category.NormalizedName.Equals(categoryNormalizedName));
		var count = query.Count();
		if (count == 0)
		{
			return new ResponseData<ListModel<Clothes>>
			{
				Data = dataList
			};
		}
		int totalPages = (int)Math.Ceiling(count / (double)pageSize);
		if (pageNo > totalPages)
		{
			return new ResponseData<ListModel<Clothes>>
			{
				Data = null,
				Success = false,
				ErrorMessage = "Нет такой страницы"

			};
		}

		dataList.Items = await query
		.Skip((pageNo - 1) * pageSize)
		.Take(pageSize)
		.ToListAsync();

		dataList.CurrentPage = pageNo;
		dataList.TotalPages = totalPages;
		var response = new ResponseData<ListModel<Clothes>>
		{
			Data = dataList
		};
		return response;
	}

	public async Task<ResponseData<Clothes>> GetClothesByIdAsync(int id)
	{
		var clothes = await _dbContext.Clothes.FindAsync(id);
		if (clothes is null)
		{
			return new ResponseData<Clothes>()
			{
				Data = null,
				Success = false,
				ErrorMessage = "Нет одежды с таким id"
			};
		}

		return new ResponseData<Clothes>()
		{
			Data = clothes
		};
	}

	public async Task UpdateClothesAsync(int id, Clothes product)
	{
		var clothes = await _dbContext.Clothes.FindAsync(id);
		if (clothes is null)
		{
			throw new ArgumentException("Нет одежды с таким id");
		}

		clothes.Name = product.Name;
		clothes.Description = product.Description;
		clothes.Price = product.Price;
		clothes.CategoryId = product.CategoryId;
		clothes.Category = product.Category;
		_dbContext.Entry(clothes).State = EntityState.Modified;
		await _dbContext.SaveChangesAsync();
	}

	public async Task DeleteClothesAsync(int id)
	{
		var clothes = await _dbContext.Clothes.FindAsync(id);
		if (clothes is null)
		{
			throw new ArgumentException("Нет одежды с таким id");
		}

		_dbContext.Clothes.Remove(clothes);
		await _dbContext.SaveChangesAsync();
	}

	public async Task<ResponseData<Clothes>> CreateClothesAsync(Clothes product)
	{
		_dbContext.Clothes.Add(product);
		try
		{
			await _dbContext.SaveChangesAsync();
		}
		catch (DbUpdateConcurrencyException ex)
		{
			return new ResponseData<Clothes>()
			{
				Data = null,
				Success = false,
				ErrorMessage = ex.Message
			};
		}
		return new ResponseData<Clothes>()
		{
			Data = product
		};
	}

	public async Task<ResponseData<string>> SaveImageAsync(int id, IFormFile formFile)
	{
		var responseData = new ResponseData<string>();
		var clothes = await _dbContext.Clothes.FindAsync(id);
		if (clothes == null)
		{
			responseData.Success = false;
			responseData.ErrorMessage = "No item found";
			return responseData;
		}
		var host = "https://" + _httpContextAccessor.HttpContext?.Request.Host;
		var imageFolder = Path.Combine(_webHostEnvironment.WebRootPath, "images");

		if (formFile != null)
		{
			if (!string.IsNullOrEmpty(clothes.ImagePath))
			{
				var prevImage = Path.GetFileName(clothes.ImagePath);
				var prevImagePath = Path.Combine(imageFolder, prevImage);
				if (File.Exists(prevImagePath))
				{
					File.Delete(prevImagePath);
				}
			}
			var ext = Path.GetExtension(formFile.FileName);
			var fName = Path.ChangeExtension(Path.GetRandomFileName(), ext);
			var filePath = Path.Combine(imageFolder, fName);
			using (var stream = new FileStream(filePath, FileMode.Create))
			{
				await formFile.CopyToAsync(stream);
			}

			clothes.ImagePath = $"{host}/images/{fName}";
			clothes.Mime = formFile.ContentType;
			_dbContext.Entry(clothes).State = EntityState.Modified;
			await _dbContext.SaveChangesAsync();
		}
		responseData.Data = clothes.ImagePath;
		return responseData;
	}
}
