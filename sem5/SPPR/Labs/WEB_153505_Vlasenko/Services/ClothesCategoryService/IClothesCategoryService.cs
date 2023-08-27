using Domain.Entities;
using Domain.Models;

namespace WEB_153505_Vlasenko.Services.ClothesCategoryService;

public interface IClothesCategoryService
{
    public Task<ResponseData<List<ClothesCategory>>> GetClothesCategoryListAsync();
}
