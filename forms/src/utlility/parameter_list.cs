// -----------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2020 Ivo Steinbrecher (https://github.com/stoani89/LaTeX2AI)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// -----------------------------------------------------------------------------

﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Xml;


namespace L2A.UTIL
{
    public class ParameterList
    {
        public ParameterList()
        {
            Initialize();
        }

        public ParameterList(string xml_path)
        {
            Initialize();

            // Check if the path exists.
            if (!File.Exists(xml_path)) L2A.ERR.ExceptionClass.Exception("The file \"" + xml_path + "\" does not exit!");

            // Parse the file to an xml object.
            FileStream fs = new FileStream(xml_path, FileMode.Open, FileAccess.Read);
            XmlDocument xdoc = new XmlDocument();
            xdoc.Load(fs);
            XmlElement docElem = xdoc.DocumentElement;

            // Get the name of the root document.
            root_name_ = docElem.Name;

            // Set the rest from the xml object.
            SetFromXml(docElem);

            // Close the file stream.
            fs.Close();
        }

        private void Initialize()
        {
            // Initialize the class variables.
            options_ = new Dictionary<string, string>();
            sub_lists_ = new Dictionary<string, ParameterList>();

            // Get the name of the root document.
            root_name_ = "";
        }

        public void SetFromXml(XmlNode xml_element)
        {
            // Add the options from this xml element.
            if (xml_element.Attributes != null)
                foreach (XmlAttribute attribute in xml_element.Attributes)
                {
                    options_[attribute.LocalName] = attribute.Value;
                }

            // Add the sublists.
            if (xml_element.ChildNodes != null)
            {
                foreach (XmlNode node in xml_element.ChildNodes)
                {
                    if (node is XmlText text)
                    {
                        // If there are no child nodes this one has a main option.
                        main_option_ = xml_element.InnerText;
                    }
                    else
                    {
                        ParameterList sub_list = new ParameterList();
                        sub_list.SetFromXml(node);
                        sub_lists_[node.Name] = sub_list;
                    }
                }
            }
        }

        public void ToXml(XmlElement xml_element)
        {
            // Add attributes.
            foreach (KeyValuePair<string, string> entry in options_)
            {
                xml_element.SetAttribute(entry.Key, entry.Value);
            }

            // Add sub lists.
            foreach (KeyValuePair<string, ParameterList> entry in sub_lists_)
            {
                XmlElement sublist = (XmlElement)xml_element.AppendChild(xml_element.OwnerDocument.CreateElement(entry.Key));
                entry.Value.ToXml(sublist);
            }

            // Check if a main option should be set.
            if (main_option_ != null)
                xml_element.InnerText = main_option_;
        }

        public string ToString(string root_name)
        {
            // Create xml document with the current structure.
            XmlDocument doc = new XmlDocument();
            XmlElement el = (XmlElement)doc.AppendChild(doc.CreateElement(root_name));
            ToXml(el);
            return doc.OuterXml;
        }

        //! Root name of this item.
        public string root_name_;

        //! Dictionary with attributes for this item.
        public Dictionary<string, string> options_;

        //! Dictionary with sub lists for this item.
        public Dictionary<string, ParameterList> sub_lists_;

        //! Main option of this item.
        public string main_option_;
    }
}
