#include "ir/module.hpp"

using namespace eviir;

/// @section Constructors

Module::Module(string name): name(name)
{
	
}

/// @section IR generation

string Module::generate_ir_comment(string text, bool header)
{
	int max_sentence_size = header ? __IR_HCOMMENT_LENGTH - 2 * __IR_HCOMMENT_MIN_SURROUND - 2
								   : __IR_COMMENT_LENGTH;

	// separate text in sentences
	vector<string> sentences = vector<string>();
	string sentence = text;
	while(sentence.length() > max_sentence_size)
	{
		// too long, find whitespace to split sentences
		int i;
		for(i = max_sentence_size - 1; i > 0 && sentence[i] != ' '; i--) {}

		sentences.push_back(tools::trim_copy(sentence.substr(0, i)));
		sentence.erase(0, i);
	}
	sentences.push_back(tools::trim_copy(sentence));

	// generate comment(s)
	if(header)
	{
		if(sentences.size() == 1)
		{
			sstream stream = sstream();

			int len = sentence.length() + 2;
			int surround_chars = __IR_HCOMMENT_LENGTH - len;
			int start_surround_chars = surround_chars / 2;
			int end_surround_chars = surround_chars - start_surround_chars;

			stream << "; " + string(start_surround_chars, __IR_HCOMMENT_SURROUND_CHAR);
			stream << " " + sentence + " ";
			stream << string(end_surround_chars, __IR_HCOMMENT_SURROUND_CHAR) << std::endl;

			return stream.str();
		}
		else
		{
			sstream stream = sstream();
			for(const string& sentence : sentences)
			{
				int end_whitespaces = __IR_HCOMMENT_LENGTH - (
					__IR_HCOMMENT_MIN_SURROUND + 1 + sentence.length()); 
				DEBUG_PRINT_F_MSG("%d - (%d + %d + %d) = %d",
					__IR_HCOMMENT_LENGTH, __IR_HCOMMENT_MIN_SURROUND, 1, sentence.length(), end_whitespaces);

				stream << "; " + string(__IR_HCOMMENT_MIN_SURROUND, __IR_HCOMMENT_SURROUND_CHAR);
				stream << ' ' + sentence + string(end_whitespaces, ' ');
				stream << string(__IR_HCOMMENT_MIN_SURROUND, __IR_HCOMMENT_SURROUND_CHAR) << std::endl;
			}
			return stream.str();
		}
	}
	else
	{
		return "; " + text;
	}
}

string Module::generate_metadata_ir()
{
	return "!module/name \"" + name + "\"";
}

string Module::generate_ir()
{
	sstream stream = sstream();

	// generate metata ir
	stream << generate_ir_comment("module metadata", true) << std::endl;
	stream << generate_metadata_ir() << std::endl;
	stream << generate_ir_comment("some header comment that is waaaaay too long to fit in one line", true) << std::endl;

	// return string
	return stream.str();
}