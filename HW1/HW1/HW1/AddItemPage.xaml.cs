using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace HW1
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class AddItemPage : ContentPage
    {
        public AddItemPage()
        {
            InitializeComponent();
        }

        private void ButtonBaikal_Clicked(object sender, EventArgs e)
        {
            MainPage.items.Add(new Item("Baikal", 1));
        }

        private void ButtonColaCola_Clicked(object sender, EventArgs e)
        {
            MainPage.items.Add(new Item("Cola Cola", 1));
        }
    }
}