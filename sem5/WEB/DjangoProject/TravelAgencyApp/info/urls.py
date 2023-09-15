from django.urls import path
from . import views

app_name = 'info'

urlpatterns = [
    path('main', views.main_view, name='main_view'),
    path('about', views.about_view, name='about_view'),
    path('news', views.news_view, name='news_view'),
    path('faq', views.faq_view, name='faq_view'),
    path('staff', views.staff_view, name='staff_view'),
    path('privacy', views.privacy_view, name='privacy_view'),
    path('review/list', views.review_list_view, name='review_list_view'),
    path('promo_codes', views.promo_codes_view, name='promo_codes_view'),
]