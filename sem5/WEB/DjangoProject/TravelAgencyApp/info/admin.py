from django.contrib import admin
from .models import News, Faq


@admin.register(News)
class NewsAdmin(admin.ModelAdmin):
    list_display = ['name', 'brief_description', 'url']


@admin.register(Faq)
class FaqAdmin(admin.ModelAdmin):
    pass
