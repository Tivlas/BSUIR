using Domain.Entities;
using Domain.Models;

namespace API.Services;

public interface IClothesCategoryService
{
	public Task<ResponseData<List<ClothesCategory>>> GetClothesCategoryListAsync();
}
