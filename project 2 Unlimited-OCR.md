**Unlimited-OCR** is an open-source, end-to-end document parsing and OCR model released by **Baidu** in June 2026. Built as an evolution of the DeepSeek-OCR lineage, its core breakthrough is the ability to process and transcribe dozens of pages of documents (or an entire book) in a **single forward pass** under a standard maximum context length of 32K without encountering memory crashes or major generation slowdowns.

Here is an analysis of its architecture, performance, strengths, and limitations.

---

### 1. Key Architectural Innovation: R-SWA

In traditional Vision Language Models (VLMs) used for long-form OCR, the Large Language Model (LLM) decoder relies on *full attention*. This means that as the output text grows, the Key-Value (KV) cache grows linearly, heavily draining GPU memory and progressively slowing down generation.

Unlimited-OCR solves this by introducing **Reference Sliding Window Attention (R-SWA)**, which mimics human working memory during a copying task:

* **Reference Tokens (Visual):** The decoder maintains full attention on all visual tokens extracted from the document image throughout the process.
* **Output Tokens (Text):** Instead of attending to every single word it has previously written, the model only attends to the preceding $n$ output tokens (default is a window of 128 tokens).
* **Result:** The KV Cache size remains **constant** throughout the entire decoding process, drastically cutting down memory usage and allowing continuous, high-speed transcription.

### 2. Model Structure & Specs

* **Lineage & Encoder:** It borrows the vision stack from DeepSeek-OCR (employing a `SAM-ViT-B` + `CLIP-L` DeepEncoder). This architecture compresses a high-resolution $1024 \times 1024$ document page into just 256 visual tokens, minimizing the initial sequence length.
* **Decoder:** It utilizes a lean **Mixture-of-Experts (MoE)** architecture with **3 Billion total parameters**, but only activates **500 Million parameters** during inference. This makes it highly efficient and runnable on consumer-grade hardware (requiring as little as a single GPU with $\ge$8 GB VRAM for BF16 inference).
* **Modes:** * `gundam` mode (optimized for single images, uses image cropping).
* `base` mode (optimized for multi-page/PDF inputs up to 32k context lengths).



### 3. Performance & Benchmarks

According to its technical report and early evaluations:

* **OmniDocBench v1.5:** It achieves a **93% score**, outperforming the DeepSeek-OCR baseline by roughly 6%.
* **Speed:** It achieves a **~35% faster throughput** on long document workloads compared to traditional full-attention OCR architectures.
* **ParseBench:** It achieves an overall mean score of **46.17**, showing excellent proficiency in extracting plain text content (scoring 86.81) and formatting structured text into clean markdown.

### 4. Ecosystem & Deployment Support

Baidu released the model with day-one integrations across popular LLM serving frameworks:

* **Hugging Face Transformers:** Fully compatible using standard `pipeline` functions (requires `trust_remote_code=True`).
* **vLLM & SGLang:** Native serving support with custom logit processors (an $n$-gram processor is required during inference to eliminate repetition loops common in long-horizon generation).
* **Format:** Outputs structured text natively, including layout detection, table understanding, and mathematical formulas markdown.

### 5. Critical Caveats & Weaknesses

Despite its massive efficiency breakthroughs, initial community feedback highlights a few distinct limitations:

* **Hallucination on Low-Quality Scans:** While it scores exceptionally well on clean, digitally structured documents, user evaluations note that on faint, blurry, or low-quality physical scans, the LLM decoder can hallucinate. Instead of leaving unreadable parts blank, it leans on its language priors to invent plausible-sounding content, reducing its strict faithfulness compared to models like `olmOCR-2`.
* **Fragile Prompts:** The inference loop requires strict formatting (e.g., the prompt must explicitly start with the literal `<image>` tag, and specialized logit processors must be active), otherwise the model may output empty text or loop infinitely on coordinate tags.

### Conclusion

Baidu's Unlimited-OCR represents a major milestone in document intelligence. By replacing vanilla attention with R-SWA, it achieves "one-shot" multi-page PDF parsing that bypasses the traditional memory walls of LLMs. It is an ideal tool for low-cost, high-throughput processing of large digital document archives, though users should exercise caution when processing heavily degraded physical scans.
