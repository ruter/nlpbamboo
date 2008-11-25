#ifndef YCAKE_TOKEN_FILTER_HXX
#define YCAKE_TOKEN_FILTER_HXX

#include "lexicon_factory.hxx"
#include "ilexicon.hxx"
#include "datrie.hxx"
#include "config_factory.hxx"
#include "utf8.hxx"

namespace bamboo { namespace ycake {

class TokenFilter {
protected:
	bamboo::ILexicon * _filter_dict;
	bool _is_init;

protected:
	int _feature_min_length;
	int _feature_min_utf8_length;

public:
	TokenFilter():_filter_dict(NULL),_is_init(false) {}
	~TokenFilter() {
		if(_filter_dict) delete _filter_dict;
	}

	int init(IConfig * config) {
		const char * s;
		config->get_value("ke_filter_dict", s);
		_filter_dict = LexiconFactory::load(s);

		config->get_value("ke_feature_min_length", _feature_min_length);
		config->get_value("ke_feature_min_utf8_length", _feature_min_utf8_length);

		_is_init = true;
		return 0;
	}

	bool is_init() {
		return _is_init;
	}

	static TokenFilter & get_instance() {
		static TokenFilter dict;
		return dict;
	}

	bool is_filter_word(const char * word) {
		if(_rule_filter(word)) {
			return true;
		}

		if(!_is_init || !_filter_dict) {
			return false;
		}

		int val = _filter_dict->search(word);

		if(val > 0)
			return true;

		return false;
	}

protected:
	bool _rule_filter(const char *token) {
		if(int(utf8::length(token)) < _feature_min_utf8_length) {
			return true;
		} else if((int)strlen(token) < _feature_min_length) {
			return true;
		}
		return false;
	}

};

}} // end of namespace bamboo::ycake

#endif // end of YCAKE_TOKEN_FILTER_HXX
