using System;
using System.Collections.ObjectModel;
using Xamarin.Forms;

namespace HW1
{
    public class Item
    {

        public Item(string item_name, int item_count)
        {
            this.item_name = item_name;
            this.item_count = item_count;
        }

        public string item_name
        {
            get; set;
        }
        
        public int item_count
        {
            get; set;
        }

        public void UpdateCount(int newCount)
        {
            item_count = newCount;
        }
    }

    public partial class MainPage : ContentPage
    {
        public static ObservableCollection<Item> items = new ObservableCollection<Item>();

        public MainPage()
        {
            InitializeComponent();
            
            BindableLayout.SetItemsSource(items_container, items);
        }

        private void Button_Clicked(object sender, EventArgs e)
        {
            Navigation.PushAsync(new AddItemPage());
        }
    }
}
