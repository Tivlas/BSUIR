from django.contrib import admin
from .models import News, Faq, Review, PromoCode, BannerChangeTimer

@admin.register(BannerChangeTimer)
class BannerChangeTimerAdmin(admin.ModelAdmin):
    list_display = ['milliseconds']
@admin.register(News)
class NewsAdmin(admin.ModelAdmin):
    list_display = ['name', 'brief_description', 'url']


@admin.register(Faq)
class FaqAdmin(admin.ModelAdmin):
    pass


@admin.register(Review)
class ReviewAdmin(admin.ModelAdmin):
    pass


@admin.register(PromoCode)
class CouponAdmin(admin.ModelAdmin):
    pass
