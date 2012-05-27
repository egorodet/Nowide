//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_CONVERT_H_INCLUDED
#define BOOST_NOWIDE_CONVERT_H_INCLUDED

#include <boost/locale/encoding_utf.hpp>
#include <string>

namespace boost {
    namespace nowide {
        ///
        /// Template function that converts a buffer of UTF sequence in range [source_begin,source_end)
        /// to the output \a buffer of size \a buffer_size.
        ///
        /// In case of success a NUL terminated string returned (buffer), otherwise 0 is returned.
        ///
        /// If there is not enough room in the buffer or the source sequence contains invalid UTF
        /// 0 is returned, and the contend of the buffer is undefined.
        ///
        template<typename CharOut,typename CharIn>
        CharOut *basic_convert(CharOut *buffer,size_t buffer_size,CharIn const *source_begin,CharIn const *source_end)
        {
            CharOut *rv = buffer;
            if(buffer_size == 0)
                return 0;
            buffer_size --;
            while(source_begin!=source_end) {
                using namespace boost::locale::utf;
                code_point c = utf_traits<CharIn>::template decode<CharIn const *>(source_begin,source_end);
                if(c==illegal || c==incomplete) {
                    rv = 0;
                    break;
                }
                size_t width = utf_traits<CharOut>::width(c);
                if(buffer_size < width) {
                    rv=0;
                    break;
                }
                buffer = utf_traits<CharOut>::template encode<CharOut *>(c,buffer);
                buffer_size -= width;
            }
            *buffer++ = 0;
            return rv;
        }

        /// \cond INTERNAL
        namespace details {
            //
            // wcslen defined only in C99... So we will not use it
            //
            template<typename Char>
            Char const *basic_strend(Char const *s)
            {
                while(*s)
                    s++;
                return s;
            }
        }
        /// \end 

        ///
        /// Convert NUL terminated UTF source string to NUL terminated \a output string of size at
        /// most output_size (including NUL)
        /// 
        /// In case of surcess output is returned, if the input sequence is illegal,
        /// or there is not enough room NULL is returned 
        ///
        inline char *convert(char *output,size_t output_size,wchar_t const *source)
        {
            return basic_convert(output,output_size,source,details::basic_strend(source));
        }
        ///
        /// Convert UTF text in range [begin,end) to NUL terminated \a output string of size at
        /// most output_size (including NUL)
        /// 
        /// In case of surcess output is returned, if the input sequence is illegal,
        /// or there is not enough room NULL is returned 
        ///
        inline char *convert(char *output,size_t output_size,wchar_t const *begin,wchar_t const *end)
        {
            return basic_convert(output,output_size,begin,end);
        }
        ///
        /// Convert NUL terminated UTF source string to NUL terminated \a output string of size at
        /// most output_size (including NUL)
        /// 
        /// In case of surcess output is returned, if the input sequence is illegal,
        /// or there is not enough room NULL is returned 
        ///
        inline wchar_t *convert(wchar_t *output,size_t output_size,char const *source)
        {
            return basic_convert(output,output_size,source,details::basic_strend(source));
        }
        ///
        /// Convert UTF text in range [begin,end) to NUL terminated \a output string of size at
        /// most output_size (including NUL)
        /// 
        /// In case of surcess output is returned, if the input sequence is illegal,
        /// or there is not enough room NULL is returned 
        ///
        inline wchar_t *convert(wchar_t *output,size_t output_size,char const *begin,char const *end)
        {
            return basic_convert(output,output_size,begin,end);
        }


    	///
    	/// Convert between Wide - UTF-16/32 string and UTF-8 string.
        ///
        /// boost::locale::conv::conversion_error is thrown in a case of a error
    	///
    	inline std::string convert(wchar_t const *s)
    	{
    		return boost::locale::conv::utf_to_utf<char>(s);
    	}
    	///
    	/// Convert between UTF-8 and UTF-16 string, implemented only on Windows platform
    	///
        /// boost::locale::conv::conversion_error is thrown in a case of a error
    	///
    	inline std::wstring convert(char const *s)
    	{
    		return boost::locale::conv::utf_to_utf<wchar_t>(s);
    	}
    	///
    	/// Convert between Wide - UTF-16/32 string and UTF-8 string
    	///
        /// boost::locale::conv::conversion_error is thrown in a case of a error
    	///
    	inline std::string convert(std::wstring const &s) 
    	{
    		return boost::locale::conv::utf_to_utf<char>(s);
    	}
    	///
    	/// Convert between UTF-8 and UTF-16 string, implemented only on Windows platform
    	///
        /// boost::locale::conv::conversion_error is thrown in a case of a error
    	///
    	inline std::wstring convert(std::string const &s) 
    	{
    		return boost::locale::conv::utf_to_utf<wchar_t>(s);
    	}

    } // nowide
} // boost

#endif
///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4