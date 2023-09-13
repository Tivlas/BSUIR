from django.contrib import admin
from django.urls import path
from django.urls import include
from django.conf.urls.static import static
from django.conf import settings

urlpatterns = [
    path('admin/', admin.site.urls),
    path('login/', include('login.urls', namespace='login')),
    path('order/', include('order.urls', namespace='order')),
    path('cart/', include('cart.urls', namespace='cart')),
    path('info/', include('info.urls', namespace='info')),
    path('personal_account/', include('personal_account.urls', namespace='personal_account')),
    path('my_statistics/', include('my_statistics.urls', namespace='my_statistics')),
    path('', include('travel.urls', namespace='travel')),
] + static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)
