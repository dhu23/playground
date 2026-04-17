// LRU (Least Recently Used) cache 
// An LRU cache is a data structure that evicts the least recently accessed key
// when the cache reaches its capacity. It behaves like a general cache (dictionary)
// and the least recently accessed key is the key that's inserted/updated/retrieved
// lesat recently


// design idea:
// 1. For a cache, we can use unordered_map to implement the key -> value relationship
// 2. The LRU ranking of keys needs to satisfy the following condition
//    - be able to quickly locate a key in the ranking
//    - move a key from the middle to the top
//    - delete a key
//    We can use doubly linked list to track suck ranking. 


#include <iostream>
#include <memory>
#include <optional>
#include <unordered_map>
template<typename K, typename V>
class LRUCache {  
    struct RankNode {
        K key;
        std::shared_ptr<RankNode> prev;
        std::shared_ptr<RankNode> next;
    };

    struct CacheNode {
        V value;
        std::shared_ptr<RankNode> node;
    };

    bool addFront(const std::shared_ptr<RankNode>& node) {
        if (node->next != nullptr || node->prev != nullptr) {
            return false;
        }
        std::shared_ptr<RankNode> currentTop = this->head_->next;
        
        node->next = currentTop;
        node->prev = head_;

        currentTop->prev = node;
        head_->next = node;
        return true;
    }

    bool removeRankNode(const std::shared_ptr<RankNode>& node) {
        if (node->next == nullptr || node->prev == nullptr) {
            return false;
        }

        std::shared_ptr<RankNode> prev = node->prev;
        std::shared_ptr<RankNode> next = node->next;

        prev->next = next;
        next->prev = prev;

        node->next = nullptr;
        node->prev = nullptr;

        return true;
    }

    void removeCacheNode(K key) {
        data_.erase(key);
    }

    int capacity_;

    // the doubly linked list for ranking
    std::shared_ptr<RankNode> head_;
    std::shared_ptr<RankNode> tail_;
    
    std::unordered_map<K, CacheNode> data_;

  public:
    LRUCache(int capacity)
        : capacity_(capacity)
        , head_{std::make_shared<RankNode>(K(), nullptr, nullptr)}
        , tail_{std::make_shared<RankNode>(K(), nullptr, nullptr)}
        , data_{} {
        // set up sentinal nodes for head and tail
        head_->next = tail_;
        tail_->prev = head_;
    }

    void put(K key, V value) {
        auto found = data_.find(key);
        if (found == data_.end()) {
            // this is a new insertion
            
            // add the key ranking node to doubly linked list first
            std::shared_ptr<RankNode> node = std::make_shared<RankNode>(key, nullptr, nullptr);
            
            // set this node as the top of the rank list, between Head and what's after head
            if (this->addFront(node)) {
                // insert into cache
                data_.emplace(key, CacheNode{value, node});

                // check if any key should be removed
                while (data_.size() > capacity_) {
                    // find the last node
                    std::shared_ptr<RankNode> currentLast = tail_->prev;
                    if (this->removeRankNode(currentLast)) {
                        this->removeCacheNode(currentLast->key);
                    }
                }
            }            
        } else {
            // this is an existing key

            CacheNode &cacheNode = found->second;
            this->removeRankNode(cacheNode.node);
            this->addFront(cacheNode.node);
            cacheNode.value = value;
        }
    }

    std::optional<V> get(K key) {
        auto found = data_.find(key);
        if (found == data_.end()) {
            return std::optional<V>();
        } else {
            CacheNode& cacheNode = found->second;
            this->addFront(cacheNode.node);
            return std::optional<V>(cacheNode.value);
        }
    }

    void remove(K key) {

    }

    bool has(K key) const {
        auto found = data_.find(key);
        return found != data_.end();
    }
};

int main(int argc, char* argv[]) {
    LRUCache<int, std::string> cache{2};

    cache.put(5, "5");
    cache.put(6, "6");

    std::cout 
        << "has 5 ->" << cache.has(5) 
        << ", has 6 ->" << cache.has(6) << std::endl;

    cache.get(5);
    cache.get(6);

    cache.put(5, "55");

    cache.put(4, "4");
    std::cout 
        << "has 4 ->" << cache.has(4)
        << ", has 5 ->" << cache.has(5) 
        << ", has 6 ->" << cache.has(6) 
        << std::endl;

    return 0;
}