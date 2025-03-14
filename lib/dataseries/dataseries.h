class DataSeries {

    private:
        float* data_array;
        int data_size;
        int data_current_index = 0;
        int data_all_indexes = 0;
        bool overrun = false;
        float series_max_value = __FLT_MIN__;
        float series_min_value = __FLT_MAX__;

        void data_index_up() {
            if (data_current_index + 1 == data_size) {
                data_current_index = 0;
                data_all_indexes++;
                overrun = true;
            } else {
                data_current_index++;
            };
        }
        
        void data_index_down() {
            if (data_current_index > 0 ) {
               data_current_index--;
            };
        }

    public:
        DataSeries(int size) {
            data_array = new float[size];
            data_size = size;
        }

        ~DataSeries() {
            delete[] data_array;
        }

        int push(float new_value) {
            int data_current_index_tmp = data_current_index; 
            data_array[data_current_index] = new_value;
            data_index_up();
            return data_current_index_tmp;
        }

        float getValueByIndex(const int ind) {
            return ( data_array[ind] );
        }

        int getMaxSize() {
            return data_size;
        }

        int getCurrentSize() {
            if (overrun) {
                return getMaxSize();
            } else {
                return data_current_index;
            };
        }

        float getSum() {
            float sum_of_values = 0;
            for (int ind = 0; ind < getCurrentSize(); ind++) {
                sum_of_values += getValueByIndex(ind);
            }
            return sum_of_values;
        }

        float getAvarage() {
            int size_to_calculate = 0;
        
            size_to_calculate = getCurrentSize();

            return (getSum() / size_to_calculate);
        }

        float getMax() {
            series_max_value = __FLT_MIN__;
            for ( int i = 0 ; i < getCurrentSize(); i++ ) {
                if ( getValueByIndex(i) > series_max_value ) {
                    series_max_value = getValueByIndex(i);
                };
            };
            return series_max_value;
        }

        float getMin() {
            series_min_value = __FLT_MAX__;
            for ( int i = 0 ; i < getCurrentSize(); i++ ) {
                if ( getValueByIndex(i) < series_min_value ) {
                    series_min_value = getValueByIndex(i);
                };
            };
            return series_min_value;
        }


};
