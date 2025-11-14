# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information
import os
import sys
<<<<<<< HEAD
sys.path.insert(0, os.path.abspath('../../'))

project = 'MuLife'
copyright = '2025, LuxNasone'
author = 'LuxNasone'
=======
sys.path.insert(0, os.path.abspath('..'))

project = 'MuLife'
copyright = '2025, LucaNasone'
author = 'LucaNasone'
>>>>>>> 3864f96 (Salvataggio temporaneo prima del pull)
release = '0.1'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

<<<<<<< HEAD
extensions = [ "sphinx.ext.autodoc",
               "sphinx.ext.napoleon",
               "sphinx_autodoc_typehints"]
=======
extensions = []
>>>>>>> 3864f96 (Salvataggio temporaneo prima del pull)

templates_path = ['_templates']
exclude_patterns = []



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'alabaster'
html_static_path = ['_static']
