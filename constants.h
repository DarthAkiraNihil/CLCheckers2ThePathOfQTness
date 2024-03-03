#include <QString>

#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace AppConst {
    const int cpuDrawChances[6] = {
        999,
        800,
        500,
        250,
        80,
        0
    };
    const QString
    standardAssetsNames[9] = {
        "board.png",
        "black.png",
        "black_king.png",
        "white.png",
        "white_king.png",
        "chosen_frame.png",
        "destination_frame.png",
        "victim_frame.png",
        "start.png"
    },
    rivalVictoryMessages[6] = {
       "\"Дукука?! Ука-бука! ХАЯБУСА!\"\n\nВы проиграли неандертальцу... как так-то? Вам необходимо его одолеть, иначе я не буду уважать вас",
        "\"Ого?! Я победил!? ДА! НАКОНЕЦ-ТО, Я СТАЛ СИЛЬНЕЕ!\"\n\nВы проиграли обычному школьнику...? Серьёзно? Что ж, вам всё ещё необходимо совершенствовать своё умение играть в шашки. Рано или поздно вы одолеете его, не сомневайтесь",
        "\"Видишь? Я лучше тебя во всём, а ты всего лишь мелкий кусок мусора, который недостоин моего уважения!\"\n\nВы проиграли лицеисту, который не самый слабый противник, потому вам не стоит отчаиваться. Вам надо обязательно утереть нос этому зазнайке, а не то он морально раздавит всех!",
        "\"Ну и что ты сделаешь? Плюхнешься на меня из-за проигрыша? Хе-хе\"\n\nК сожалению, вы проиграли Тайлунгу. Могу сказать только одно - совершенствуйте своё мастерство игры в шашки, вам нужно показать ему, кто здесь - воин дракона!",
        "\"Yare yare daze!\"\n\nХм... А чего вы ожидали, собираясь играть против Джотаро Куджо? Он силён, и этим всё сказано. Потренируйтесь играть в шашки побольше и попробуйте ещё раз.",
        "\"АААААХАХАХАХА! ТАКИ ТЫ ОПОЗДАЛ СОНИК! Я ТЕПЕРЬ ОКОНЧАТЕЛЬНО СЕРТИФИЦИРОВАЛСЯ КАК ПРОГРАММА ДЛЯ ИГРЫ В РУССКИЕ ШАШКИ! ТЕПЕРЬ ТЫ МЕНЯ НЕ ОСТАНОВИШЬ!\"\n\nО НЕТ! Вы проиграли доктору Эггману, и теперь он превратит всех в бандиков! СРОЧНО, улучшайте своё мастерство игры в шашки и вызывайте доктора на матч-реванш!"


    },
    surrenderMessages[6] = {

    },
    drawAcceptMessages[6] = {
        "\"Хумука... Бука-дука кука! Нуука!\"\n\nХм, оппонент принял ваше предложение ничьи. Победила дружба.",
        "\"Хм, если вам так хочется, то... я принимаю предложение ничьи!\"\n\nХм, оппонент принял предложение ничьи. Победила дружба.",
        "\"Гм, я согласен на ничью... чтобы потом усилить свой навык и прикончить тебя в следующей партии\"\n\nХм, зазнайка принял предложение ничьи. Победила дружба.",
        "\"Передай мастеру Шифу, что это не последняя наша встреча!\"\n\nХм, странно, но оппонент принял предложение ничьи. Победила дружба.",
        "\"Я потом выбью из тебя всё де...о, если в следующий рад подойдёшь ближе к победе надо мной\"\n\nХм, удивительно, но Джотаро Куджо принял предложение ничьи. Победила дружба.",
        "Something went wrong... what?"
    },
    drawDeclineMessages[6] = {
        "\"Нуука? Нону бука гуга суда лукака!\"\n\nЭто удивительно, но неандерталец не принял ваше предложение ничьи :(",
        "\"Хм, не принимаю предложение.\"\n\nВаш оппонент не принял ваше предложение ничьи :(",
        "\"Ещё чего! Я уже почти победил, и я не буду уступать какому-то отбросу!\"\n\nЗазнайка не принял ваше предложение ничьи :(",
        "\"Я знаю, что ты не воин дракона, но отступить я не имею право.\"\n\nВаш оппонент не принял ваше предложение ничьи :(",
        "\"Ты что, смеёшься надо мной? Я поставил душу своей матери! Я должен одолеть тебя!\"\n\nВаш оппонент не принял ваше предложение ничьи :(",
        "\"ААААА! ЧТО? ТЫ СЕРЬЁЗНО ДУМАЛ, ЧТО Я СОГЛАШУСЬ НА НИЧЬЮ?!!! НУ УЖ НЕТ! МОЙ ПЛАН ОСУЩЕСТВИТСЯ И Я ОКОНЧАТЕЛЬНО СЕРТИФИЦИРУЮСЬ КАК ПРОГРАММА ДЛЯ ИГРЫ В РУССКИЕ ШАШКИ! БАУХА-ХА-ХА-ХА!\"\n\nА чего вы хотели? Доктор Айво \"Эггман\" Роботник непреклонен, потому и не принял ваше предложение ничьи :("
    };/*,
    rivalLosingMessages[8] = {

    }*/;
}

#endif // CONSTANTS_H
