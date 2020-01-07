#pragma once

#include <filesystem>
#include "string"
#include "map"
#include "iostream"


namespace Kit
{
    namespace fs = std::filesystem;

    using std::string;
    using std::vector;
    using std::map;

    enum class font_weight
    {
        ULTRATHIN,
        THIN,
        LIGHT,
        REGULAR,
        MEDIUM,
        SEMIBOLD,
        BOLD,
        EXTRABOLD,
        BLACK
    };

    enum class font_style
    {
        NORMAL,
        ITALIC
    };


    class font_cache
    {
    private:
        string _font_name;
        font_style _font_style;
        font_weight _font_weight;

        string _path;

    public:
        font_cache(const string& _font_name, font_style _font_style, font_weight _font_weight, const string& _path);

    public:
        bool operator==(const font_cache& cache);

        string path();
    };


    class font_find
    {
        string _font_name;
        font_style _font_style;
        font_weight _font_weight;

        map<string, string> _equivalent_names;

        size_t _count_font_weights;
        vector<vector<string> > _font_weight_string;

        vector<bool> _font_weight_found;
        vector<fs::path> _font_weight_found_path;


        /** Cache */
        static vector<font_cache> _cache;

    public:
        font_find(const string& font_name, font_style font_style, font_weight font_weight);

    private:
        static string to_lower(const string& str);

    public:
        string find();

        static void delete_cache();
    };

}