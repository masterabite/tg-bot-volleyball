#include <modules.h>

Predictor::Predictor() {
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::srand(static_cast<unsigned>(seed));
}

std::string Predictor::predict_date() {
	time_t now = time(0);
    size_t 
        shiftYear   = 0,
        shiftMonth  = 0,
        shiftDay    = 0
    ;

    if (rand()%3) { 
        if (rand()%3) {
            if (rand()%3) {
                shiftYear = rand()%4; 
            } else {
                shiftYear = rand()%10; 
            } 
        } else {
            shiftYear = rand()%100; 
        }
    } 
    if (rand()%3) { shiftMonth = rand()%12; } 
    if (rand()%3) { shiftDay = rand()%31; } 

	std::time_t predictTime = now + ((shiftYear*12 + shiftMonth)*30 + shiftDay)*60*60*24;
	return Dater::get_date_string(predictTime);
}


std::string Predictor::da_net() {
    danet = {
        "а тебя это волновать не должно, большие города...", 
        "вероятность крайне мала", "спросите снова", 
        "ты точно хочешь знать?", "забудь об этом", 
        "да", "нет", "возможно", "есть сомнения", 
        "неясно", "точно да", "духи говорят да", 
        "как карта ляжет, как кент расскажет"
        "без сомнений", "думаю что нет", 
        "мне нельзя на это отвечать", 
    };
    return danet[rand()%danet.size()];
}