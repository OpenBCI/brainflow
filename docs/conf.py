# -*- coding: utf-8 -*-
#
# Configuration file for the Sphinx documentation builder.
#
# This file does only contain a selection of the most common options. For a
# full list see the documentation:
# http://www.sphinx-doc.org/en/master/config

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import sys
import subprocess

sys.path.insert(0, os.path.abspath('.'))
sys.path.insert(0, os.path.abspath(os.path.join('..', 'python-package')))
sys.path.insert(0, os.path.abspath(os.path.join('..', 'python-package', 'brainflow')))

# -- Project information -----------------------------------------------------

project = u'BrainFlow'
copyright = u'2019, Andrey Parfenov'
author = u'Andrey Parfenov'

# The short X.Y version
version = u''
# The full version, including alpha/beta/rc tags
release = u''


# -- General configuration ---------------------------------------------------

# If your documentation needs a minimal Sphinx version, state it here.
#
# needs_sphinx = '1.0'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autodoc',
    'breathe',
    'sphinxcontrib.ghcontributors',
    'nbsphinx',
    'sphinx.ext.mathjax'
]

# Breathe and Doxygen setup

breathe_default_project = 'BrainFlowCpp'

def configure_doxyfile(input_dir, output_dir, project):
    with open('Doxyfile.in', 'r') as file :
        filedata = file.read()

    filedata = filedata.replace('@DOXYGEN_INPUT_DIR@', input_dir)
    filedata = filedata.replace('@DOXYGEN_OUTPUT_DIR@', output_dir)
    filedata = filedata.replace('@DOXYGEN_PROJECT@', project)

    with open('Doxyfile', 'w') as file:
        file.write(filedata)

# Check if we're running on Read the Docs' servers
read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'

breathe_projects = {}

if read_the_docs_build:
    # cpp binding
    input_dir = '../cpp-package/src'
    output_dir = 'build-cpp'
    configure_doxyfile(input_dir, output_dir, 'BrainFlowCpp')
    subprocess.call('doxygen', shell=True)
    breathe_projects['BrainFlowCpp'] = output_dir + '/xml'
    # java binding
    input_dir = '../java-package'
    output_dir = 'build-java'
    configure_doxyfile(input_dir, output_dir, 'BrainFlowJava')
    subprocess.call('doxygen', shell=True)
    breathe_projects['BrainFlowJava'] = output_dir + '/xml'
    # c# binding
    input_dir = '../csharp-package'
    output_dir = 'build-csharp'
    configure_doxyfile(input_dir, output_dir, 'BrainFlowCsharp')
    subprocess.call('doxygen', shell=True)
    breathe_projects['BrainFlowCsharp'] = output_dir + '/xml'
    # core api, we dont use it right now but maybe later we will docs for developers too
    input_dir = '../src'
    output_dir = 'build-core'
    configure_doxyfile(input_dir, output_dir, 'BrainFlowCore')
    subprocess.call('doxygen', shell=True)
    breathe_projects['BrainFlowCore'] = output_dir + '/xml'


# sphinx.ext.autodoc
autoclass_content = 'both'
autodoc_default_options = {'members': None}
autodoc_member_order = 'bysource'


# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# The suffix(es) of source filenames.
# You can specify multiple suffix as a list of string:
#
# source_suffix = ['.rst', '.md']
source_suffix = '.rst'

# The master toctree document.
master_doc = 'index'

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set 'language' from the command line for these cases.
language = None

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = [u'_build', 'Thumbs.db', '.DS_Store', '**.ipynb_checkpoints']

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = None

# options for nbsphinx
html_scaled_image_link = False
html_sourcelink_suffix = ''

nbsphinx_execute_arguments = [
    "--InlineBackend.figure_formats={'svg', 'pdf'}",
    "--InlineBackend.rc={'figure.dpi': 96}",
]
nbsphinx_input_prompt = 'In [%s]:'
nbsphinx_output_prompt = 'Out[%s]:'
nbsphinx_timeout = 60
# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#

import sphinx_rtd_theme

extensions.append('sphinx_rtd_theme')
html_theme_path = [sphinx_rtd_theme.get_html_theme_path()]
html_theme = 'sphinx_rtd_theme'

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
#
html_translator_class = 'guzzle_sphinx_theme.HTMLTranslator'
html_show_sourcelink = False

html_logo = '_static/brainflow_logo.png'

html_theme_options = {
    'canonical_url': 'https://brainflow.readthedocs.io/en/latest/',
    'collapse_navigation': False,
    'display_version': True,
    'logo_only': False,
}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named 'default.css' will overwrite the builtin 'default.css'.
html_static_path = ['_static']
html_context = {'css_files': ['_static/custom.css']}
# Custom sidebar templates, must be a dictionary that maps document names
# to template names.
#
# The default sidebars (for documents that don't match any pattern) are
# defined by theme itself.  Builtin themes are using these templates by
# default: ``['localtoc.html', 'relations.html', 'sourcelink.html',
# 'searchbox.html']``.
#
# html_sidebars = {}


# -- Options for HTMLHelp output ---------------------------------------------

# Output file base name for HTML help builder.
htmlhelp_basename = 'BrainFlowDoc'


# -- Options for LaTeX output ------------------------------------------------

latex_elements = {
    # The paper size ('letterpaper' or 'a4paper').
    #
    # 'papersize': 'letterpaper',

    # The font size ('10pt', '11pt' or '12pt').
    #
    # 'pointsize': '10pt',

    # Additional stuff for the LaTeX preamble.
    #
    # 'preamble': '',

    # Latex figure (float) alignment
    #
    # 'figure_align': 'htbp',
}

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title,
#  author, documentclass [howto, manual, or own class]).
latex_documents = [
    (master_doc, 'BrainFlow.tex', u'BrainFlow Documentation',
     u'Andrey Parfenov', 'manual'),
]


# -- Options for manual page output ------------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [
    (master_doc, 'brainflow', u'BrainFlow Documentation',
     [author], 1)
]


# -- Options for Texinfo output ----------------------------------------------

# Grouping the document tree into Texinfo files. List of tuples
# (source start file, target name, title, author,
#  dir menu entry, description, category)
texinfo_documents = [
    (master_doc, 'BrainFlow', u'BrainFlow Documentation',
     author, 'BrainFlow', 'One line description of project.',
     'Miscellaneous'),
]


# -- Options for Epub output -------------------------------------------------

# Bibliographic Dublin Core info.
epub_title = project

# The unique identifier of the text. This can be a ISBN number
# or the project homepage.
#
# epub_identifier = ''

# A unique identification for the text.
#
# epub_uid = ''

# A list of files that should not be packed into the epub file.
epub_exclude_files = ['search.html']
