# Tamizhi-
Native compiled


​🚀 தமிழி (Tamizhi) Programming Language
தமிழி (Tamizhi) என்பது தமிழ் மொழியின் எளிமையையும், கம்ப்யூட்டரின் அதிவேகத்தையும் இணைக்கும் ஒரு நவீன Compiled Programming Language. இது வெறும் ஒரு Interpreter கிடையாது; இது நேரடியா மெஷின் கோடாக (Binary) மாறும் திறன் கொண்ட ஒரு அதிவேக மொழி.
✨ சிறப்பம்சங்கள் (Features)
LLVM Backend: நவீன கம்ப்யூட்டர்களுக்கு ஏற்ப கோடை ஆப்டிமைஸ் செய்து அதிவேக அவுட்புட் தரும்.
Native CLI: C மொழியில் உருவாக்கப்பட்ட பிரத்யேக tamizhi கமாண்ட் லைன் டூல்.
Performance: Python போன்ற மொழிகளை விட பல மடங்கு வேகம் (C-Speed).
Tamil Syntax: தமிழிலேயே லாஜிக் எழுதும் வசதி (எ.கா: அச்சிடு, முழுஎண்).
Linux Native: Linux (Arch/Manjaro) மற்றும் Android (Termux) சிஸ்டம்களில் சிறப்பாக இயங்கும்.
🛠️ நிறுவும் முறை (Installation)
முதலில் உங்கள் கணினியில் gcc, clang மற்றும் llvm இருப்பதை உறுதி செய்து கொள்ளவும்.


புராஜெக்டை குளோன் செய்யவும்:

git clone https://github.com/Prabakaran202/Tamizhi.git
cd Tamizhi



கம்பைல் செய்ய (Build):

make

சிஸ்டம் கமாண்டாக மாற்ற (Global Install):

sudo cp tamizhi tamizhi_core /usr/local/bin/


🚀 முதல் தமிழி கோட் (Your First Program)
ஒரு புதிய கோப்பை உருவாக்கவும் (எ.கா: வணக்கம்.tz):


// தமிழி V0.1 Example
Num அ = 100;
Num ஆ = 200;
Num இ = அ + ஆ;

அச்சிடு இ;


இயக்க (To Run):

tamizhi run வணக்கம்.tz




📂 கோப்பு அமைப்பு (Project Structure)
src/: லெக்சர், பார்ஸர் மற்றும் கோட்-ஜென் (C source files).
include/: ஹெடர் கோப்புகள்.
examples/: தமிழி லாங்குவேஜ் சாம்பிள் கோட்கள்.
tamizhi: பயனர் பயன்படுத்தும் CLI டூல்.
tamizhi_core: தமிழியின் கம்பைலர் என்ஜின்.
🗺️ வருங்காலத் திட்டம் (Roadmap)
[x] LLVM Integration (Core Engine)
[x] Native C CLI
[ ] Conditionals (எனில் / இல்லையெனில் - if/else)
[ ] Python Library Bridge (Translation)
[ ] Tamizhi Package Manager (TPM)
🤝 பங்களிக்க (Contributing)
தமிழி ஒரு ஓப்பன் சோர்ஸ் புராஜெக்ட். இதன் வளர்ச்சிக்கு உதவ விரும்புபவர்கள் Pull Requests அனுப்பலாம் அல்லது பிழைகளை (Issues) சுட்டிக்காட்டலாம்.
Community: Backend Developer Hub (BDH)
📜 உரிமம் (License)
இந்த புராஜெக்ட் MIT License-ன் கீழ் வெளியிடப்பட்டுள்ளது.
Developed with ❤️ by Prabakaran தமிழுக்கும் அமுதென்று பேர், அந்தத் தமிழ் இன்பத் தமிழ் எங்கள் உயிருக்கு நேர்!









