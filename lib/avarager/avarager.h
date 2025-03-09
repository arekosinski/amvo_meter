class Avareager {

    private:
        float* data_array;
        int data_size;
        int data_current_index = 0;
        bool overrun = false;
        float current_max_value = __FLT_MIN__;
        float current_min_value = __FLT_MAX__;

        void data_index_up() {
            if (data_current_index + 1 == data_size) {
                data_current_index = 0;
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

        float setMaxValue(float value) {
            if ( value > current_max_value) {
                current_max_value = value;                
            };
            return current_max_value;
        }

        float setMinValue(float value) {
            if ( value < current_min_value) {
                current_min_value = value;                
            };
            return current_min_value;
        }


    public:
        Avareager(int size) {
            data_array = new float[size];
            data_size = size;
        }

        ~Avareager() {
            delete[] data_array;
        }

        void push(float new_value) {
            data_array[data_current_index] = new_value;
            data_index_up();
            setMaxValue(new_value);
            setMinValue(new_value);
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
            int size_to_calculate = 0;
        
            size_to_calculate = getCurrentSize();

            float sum_of_values = 0;

            for( int ind = 0 ; ind < size_to_calculate; ind++) {
                sum_of_values = sum_of_values + getValueByIndex(ind);
            };
            return sum_of_values;
        }

        float getAvarage() {
            int size_to_calculate = 0;
        
            size_to_calculate = getCurrentSize();

            return (getSum() / size_to_calculate);
        }

        float getMaxValue() {
            return current_max_value;
        }

        float getMinValue() {
            return current_min_value;
        }


};
