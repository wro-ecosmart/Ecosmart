import requests
import re
from gensim.summarization import summarize as gensim_summarize
def search_wikipedia(query):
    # Define the Wikipedia API search endpoint
    endpoint = "https://en.wikipedia.org/w/api.php"
    # Define the parameters for the search request
    params = {
        "action": "query",
        "list": "search",
        "format": "json",
        "srsearch": query,  # The search query
        "srlimit": 1,      # Limit the result to the first match
    }

    # Make the API search request
    response = requests.get(endpoint, params=params)

    if response.status_code == 200:
        data = response.json()
        search_results = data["query"]["search"]
        
        # Check if there are search results
        if search_results:
            first_result = search_results[0]
            title = first_result["title"]
            return title
        else:
            print("No results found for the query:", query)
            return None
    else:
        print("Search failed. Status code:", response.status_code)
        return None


def get_wikipedia(title, char_limit=None, summary_ratio=0.2):
    # Define the Wikipedia API endpoint
    endpoint = "https://en.wikipedia.org/w/api.php"
    title = search_wikipedia(title)
    # Define the parameters for the API request
    params = {
        "action": "query",
        "format": "json",
        "titles": title,
        "prop": "extracts",
        "exintro": True,
    }

    # Make the API request
    response = requests.get(endpoint, params=params)

    if response.status_code == 200:
        data = response.json()
        page_content = next(iter(data["query"]["pages"].values()))["extract"]
        
        # Remove HTML tags using regular expressions
        page_content = re.sub(r'<.*?>', '', page_content)
        
        # Summarize the content
        if char_limit is not None:
            summarized_content = gensim_summarize(page_content, word_count=char_limit)
        else:
            summarized_content = gensim_summarize(page_content, ratio=summary_ratio)
        
        return summarized_content
    else:
        print("Failed to retrieve data. Status code:", response.status_code)
        return None

# Example usage with a summary ratio:
if __name__ == "__main__":
    title = "Elon Musk"
    summary = get_wikipedia(title, summary_ratio=0.08)

    if summary:
        print(summary)
