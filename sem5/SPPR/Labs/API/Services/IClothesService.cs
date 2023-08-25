using Domain.Entities;
using Domain.Models;

namespace API.Services;

public interface IClothesService
{
	public Task<ResponseData<ListModel<Clothes>>> GetClothesListAsync(string? categoryNormalizedName, int pageNo = 1,
		int pageSize = 3);

	public Task<ResponseData<Clothes>> GetClothesByIdAsync(int id);

	public Task UpdateClothesAsync(int id, Clothes product);

	public Task DeleteClothesAsync(int id);

	public Task<ResponseData<Clothes>> CreateClothesAsync(Clothes product);

	public Task<ResponseData<string>> SaveImageAsync(int id, IFormFile formFile);
}
