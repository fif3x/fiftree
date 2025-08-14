#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
	const std::string EXE_MIS = ".exe";
	const std::string MOVE = "move";
	const std::string MKDIR = "mkdir";
#elif defined(__linux__) || defined(__unix__)
	const std::string EXE_MIS = ".out";
	const std::string MOVE = "mv";
	const std::string MKDIR = "mkdir -p";
#else
	#error Unsupported platform - Only Windows or Linux is allowed
#endif

enum Mode{
	cmd,
	compile,
	change,
	procdone
};

bool is(size_t par) {
	if (par != std::string::npos) {
		return true;
	}
	else {
		return false;
	}
}
void ttc(std::string &temporary, const char* &command) {
	command = temporary.c_str();
}
void log(const char* str, Mode mode, std::string oldcmd, std::string newcmd){
	std::cout << "[+] ";
	if(mode == Mode::cmd){
		std::cout << "Command ran: " << str << std::endl;
	}else if(mode == Mode::compile){
		std::cout << "Project compiled" << std::endl;
	}else if(mode == Mode::change){
		std::cout << oldcmd << " changed to " << newcmd << std::endl;
	}else if(mode == Mode::procdone){
		std::cout << "Process complete" << std::endl;
	}else{
		
	}
}

int main(int argc, char* argv[]) {
	if (argc > 1) {
		std::ifstream file(argv[1]);

		std::string text 							=   { };
		std::string compile 						=	{ };
		std::string binName 						=	{ };
		std::string temp 							=   { };
		std::string dirName 						=   { };
		bool fBINexists 							=   false;
		bool fSOURCEexists 							=   false;
		bool fLIBexists 							=   false;
		bool fSCRIPTSexists 						=   false;
		bool fMSexists 								=   false;
		bool fUWexists 								=   false;
		std::vector<std::string> instruc 			=   { };
		bool in_folder 								=   false;
		std::string folderToMove 					=   { };
		std::vector<std::string> sourcesToCompile   =   { };
		std::string prevCompile 					=   { };
		std::string filestr 						=	argv[1];
		auto ftr									= 	filestr.find(".ftr");
		auto txt									=	filestr.find(".txt");

		if(!is(ftr) && !is(txt)){
			file.close();
			std::cout << "ERROR: Unsupported file type, please read the manual.\n";
			return 0;
		}
		while (std::getline(file, text)) {
			instruc.push_back(text);
			text = { };
		}

		file.close();
		binName = instruc.at(0);

		if (instruc.at(1) == "CPP") {
			compile = "g++ source/main.cpp";
		}
		else if (instruc.at(1) == "C") {
			compile = "gcc source/main.c";
		}
		else {
			std::cerr << "ERROR: Unsupported language; Please check the manual\n";
			std::cin.get();
			return 1;
		}
		
		for (int i = 3; i < instruc.size(); i++) {
			auto ms_it     =   instruc[i].find("MS_");
			auto uw_it     =   instruc[i].find("UW_");
			auto cpp_it    =   instruc[i].find(".cpp");
			auto c_it      =   instruc[i].find(".c");
			auto stc_it    =   instruc[i].find("STC");
			auto h_it      =   instruc[i].find(".h");
			auto hpp_it    =   instruc[i].find(".hpp");
			auto bin_it    =   instruc[i].find(".exe") || instruc[i].find(".bin") || instruc[i].find(".out") || instruc[i].find(".aef");
			auto script_it =   instruc[i].find(".bat") || instruc[i].find(".sh") || instruc[i].find(".py") || instruc[i].find(".lua") || instruc[i].find(".cmd");
			auto f_s_it    =   instruc[i].find("/");
			auto f_e_it    =   instruc[i].find(".~");
			auto cmnt_it   =   instruc[i].find(";");
			

			const char* cmd = { };

			if(instruc[i] == "" || instruc[i] == " " || instruc[i] == "\n"){
				continue;
			}

			if (is(ms_it)) {
				std::string filename = instruc[i];
				if(filename.substr(0, 3) == "MS_"){
						filename = filename.substr(3);
				}
				if (fMSexists) {

				}
				else {
					fMSexists = true;
					temp = MKDIR + " Microsoft";
					ttc(temp, cmd);
					system(cmd);
					log(cmd, Mode::cmd, " ", " ");
				}
				temp = (MOVE + " " + filename + " Microsoft/").c_str();
				ttc(temp, cmd);
				system(cmd);
				log(cmd, Mode::cmd, " ", " ");
			}else if(is(cmnt_it)){
				continue;
			}
			else if (is(uw_it)) {
				std::string filename = instruc[i];
				if(filename.substr(0, 3) == "UW_"){
					filename = filename.substr(3);
				}
				if (fUWexists) {

				}
				else {
					fUWexists = true;
					temp = MKDIR + " Unwanted";
					ttc(temp, cmd);
					system(cmd);
					log(cmd, Mode::cmd, " ", " ");
				}
				temp = (MOVE + " " + filename + " Unwanted/").c_str();
				ttc(temp, cmd);
				system(cmd);
				log(cmd, Mode::cmd, " ", " ");
			}
			else if (is(cpp_it) || is(c_it)) {
				if (fSOURCEexists) {

				}
				else {
					fSOURCEexists = true;
					temp = MKDIR + " source";
					ttc(temp, cmd); 
					system(cmd);
					log(cmd, Mode::cmd, " ", " ");
				}
				if(is(stc_it)){
					std::string filename = instruc[i];
					if(filename.substr(0, 4) == "STC_"){
						filename = filename.substr(4);
					}
					sourcesToCompile.push_back("source/" + filename);
					temp = (MOVE + " " + filename + " source/").c_str();
				}else{
					temp = (MOVE + " " + instruc[i] + " source/").c_str();
				}
				ttc(temp, cmd);
				system(cmd);
				log(cmd, Mode::cmd, " ", " ");
			}
			else if (is(h_it) || is(hpp_it)) {
				if(fLIBexists){

				}else{
					fLIBexists = true;
					temp = MKDIR + " lib";
					ttc(temp, cmd);
					system(cmd);
					log(cmd, Mode::cmd, " ", " ");
				}
				temp = (MOVE + " " + instruc[i] + " lib/").c_str();
				ttc(temp, cmd);
				system(cmd);
				log(cmd, Mode::cmd, " ", " ");
				if(in_folder){
					temp = (MOVE + " " + instruc[i] + folderToMove).c_str();
					ttc(temp, cmd);
					system(cmd);
					log(cmd, Mode::cmd, " ", " ");
				}
			}else if(is(bin_it)){
				if (fBINexists) {
					
				}
				else {
					fBINexists = true;
					temp = MKDIR + " bin";
					ttc(temp, cmd);
					system(cmd);
					log(cmd, Mode::cmd, " ", " ");
				}
				temp = (MOVE + " " + instruc[i] + " bin/").c_str();
				ttc(temp, cmd);
				system(cmd);
				log(cmd, Mode::cmd, " ", " ");
			}
			else if (is(script_it)) {
				if (fSCRIPTSexists) {

				}
				else {
					fSCRIPTSexists = true;
					temp = MKDIR + " scripts";
					ttc(temp, cmd);
					system(cmd);
					log(cmd, Mode::cmd, " ", " ");
				}
				temp = (MOVE + " " + instruc[i] + " scripts/").c_str();
				ttc(temp, cmd);
				system(cmd);
				log(cmd, Mode::cmd, " ", " ");
			}
			else if (is(f_s_it)) {
				dirName = instruc[i].substr(0, instruc[i].find("/"));
				temp = (MKDIR + " " + dirName).c_str();
				folderToMove = dirName;
				in_folder = true;
				ttc(temp, cmd);
				system(cmd);
				log(cmd, Mode::cmd, " ", " ");

			}else if (is(f_e_it)){
				in_folder = false;
			}
			else {
				std::cout << "ERROR: Unkown Instruction; Please read the manual\n";
				std::cout << "Note: You can NOT compile both C and C++. \n";
				break;
			}
		}
		for(int j = 0; j < sourcesToCompile.size(); j++){
			temp = sourcesToCompile.at(j);
			prevCompile = compile;
			compile = prevCompile + " " + temp;
			log("", Mode::change, prevCompile, compile);
		}
		prevCompile = compile;
		compile = prevCompile + " -o " + binName;
		system(compile.c_str());
		log(compile.c_str(), Mode::cmd, "", "");
		log("", Mode::compile, "", "");
		if(fBINexists){
			const char* cmd = { };
			temp = (MOVE + " " + binName + EXE_MIS + " bin/").c_str();
			ttc(temp, cmd);
			system(cmd);
			log(cmd, Mode::cmd, "", "");
		}else{
			const char* cmd = { };
			temp = (MKDIR + " bin").c_str();
			ttc(temp, cmd);
			system(cmd);
			log(cmd, Mode::cmd, "", "");
			temp = (MOVE + " " + binName + EXE_MIS + " bin/").c_str();
			ttc(temp, cmd);
			system(cmd);
			log(cmd, Mode::cmd, "", "");
		}
		log("", Mode::procdone, "", "");
	}else{
		std::cout << "ERROR: Unexpected amount of flags; Please read the manual\n";
	}

	return 0;
}