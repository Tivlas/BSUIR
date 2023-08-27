using Domain.Entities;
using Domain.Models;

namespace WEB_153505_Vlasenko.Services.ClothesService;

public interface IClothesService
{
    public Task<ResponseData<ListModel<Clothes>>> GetClothesListAsync(string?
    categoryNormalizedName, int pageNo = 1);

    public Task<ResponseData<Clothes>> GetClothesByIdAsync(int id);

    public Task UpdateClothesAsync(int id, Clothes product, IFormFile? formFile);

    public Task DeleteClothesAsync(int id);

    public Task<ResponseData<Clothes>> CreateClothesAsync(Clothes product, IFormFile?
    formFile);
}
