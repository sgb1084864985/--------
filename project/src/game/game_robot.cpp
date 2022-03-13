// You can modify the prototypes and update them in game.h.

#include <array>
#include <string>

#include "clockint.h"
#include "card.h"
#include "rule.h"
#include "game.h"

using namespace std;

int get_score(const CardSet& cards) { // bid 1 if score > 58 bid 1; bid 2 if score > 62 ; bid 3 if score >68 // pass if score < 53
	int sum[15] = { 0 };
	int weight[15] = { 30,25,26,1,2,3,4,5,6,7,8,9,10,11,40};
	int w_order[15] = { 3,4,5,6,7,8,9,10,11,12,13,1,2,0,14 };
	int score = 0;
	int board_value = 0;
	int sequence_value = 0;
	int b_total = 0, s_total = 0, total = 0;
	int sqs = 0;

	for (int i = 1; i < 14; i++) {
		for (int j = 0; j < 4; j++) {
			sum[i] += cards.GetNum(j, i);
		}
		total += sum[i];
	}
	sum[0] = cards.GetNum(4, 0);
	sum[14] = cards.GetNum(4, 14);

	for (int i = 0; i < 15; i++) {
		board_value += sum[i] * weight[i];
		b_total += 8 * weight[i];
	}

	//for (int i = 1; i < 15; i++) {
	//	sequence_value += min(sum[w_order[i - 1]], sum[w_order[i]]) * weight[w_order[i]];
	//	s_total += 8 * weight[w_order[i]];
	//}
	for (int i = 1; i < 15; i++) {
		sequence_value += sqs;
		s_total += i-1;
		if (min(sum[w_order[i - 1]], sum[w_order[i]]) > 0) sqs += 1;
		else sqs = -1;
	}

	int scr = (board_value * 3 + sequence_value * 7-2*total) * 1000 / (1500+ 10*total) + rand()%300;
	return scr;
}

#define show_card(card_set) for (int i = 0; i < card_set.GetNumOfCards(); i++) {cerr << "(" << card_set.GetCard(i).GetSuit() << ", " << card_set.GetCard(i).GetRank() << ") ";} cerr << endl;

// n 当前已经选择了多少张牌
// last_n 上一轮出了多少张牌
// first 现在从第几张牌开始选
void SearchBest(const Rule& rule, const CardSet& my_card, const Analysis& last, CardSet& to_dealt, int n, int first, int &max_score) {
	static CardSet cur_card;
	if (n == 0) {
		cur_card = CardSet(0);
	}
	if (n > 5) {
		return;
	}
	if (Analysis(cur_card, rule) > last) {
		CardSet whats_left = my_card;
		whats_left.Delete(cur_card);
		int new_score = get_score(whats_left);
		if (new_score > max_score) {
			max_score = new_score;
			to_dealt = cur_card;
		}
	}
	int num_my_cards = my_card.GetNumOfCards();
	for (int i = first; i < num_my_cards; i++) {
		cur_card.Insert(my_card.GetCard(i));
		SearchBest(rule, my_card, last, to_dealt, n + 1, i + 1, max_score);
		cur_card.Delete(my_card.GetCard(i));
	}
}

#include <assert.h>

CardSet DouDizhuGame::playout_robot(ci<3> k) const {
	int max_score = 0;
	CardSet result(0);
	SearchBest(r_1v2_wiki, cards[k], last, result, 0, 0, max_score);
	int chance = rand() % 100;
	if (max_score < 300 && chance < 30 && last.GetFlag()) {
		return CardSet(0);
	}
	return result;
}

CardSet SirenDouDizhuGame::playout_robot(ci<4> k) const {
	int max_score = 0;
	CardSet result(0);
	SearchBest(r_1v3, cards[k], last, result, 0, 0, max_score);
	int chance = rand() % 100;
	if (max_score < 300 && chance < 30 && last.GetFlag()) {
		return CardSet(0);
	}
	return result;
}

CardSet ShuangkouGame::playout_robot(ci<4> k) const {
	int max_score = 0;
	CardSet result(0);
	SearchBest(r_2v2, cards[k], last, result, 0, 0, max_score);
	int chance = rand() % 100;
	if (max_score < 300 && chance < 30 && last.GetFlag()) {
		return CardSet(0);
	}
	return result;
}

int DouDizhuGame::bid_robot(ci<3> k) const {
	if (get_score(cards[k]) > 250) {
		return stake + 1;
	}
	return 0;
}

int SirenDouDizhuGame::bid_robot(ci<4> k) const {
	if (get_score(cards[k]) > 250) {
		return stake + 1;
	}
	return 0;
}
