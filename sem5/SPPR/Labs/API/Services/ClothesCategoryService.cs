using API.Data;
using Domain.Entities;
using Domain.Models;
using Microsoft.EntityFrameworkCore;

namespace API.Services;

public class ClothesCategoryService : IClothesCategoryService
{
    private readonly AppDbContext _dbContext;

    public ClothesCategoryService(AppDbContext dbContext)
    {
        _dbContext = dbContext;
    }

    public async Task<ResponseData<List<ClothesCategory>>> GetClothesCategoryListAsync()
    {
        var categories = _dbContext.ClothesCategory.ToListAsync();
        return new ResponseData<List<ClothesCategory>>()
        {
            Data = await categories
        };
    }
}
